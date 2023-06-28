#ifdef WIN32

#include <winsock2.h>

#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <map>

#include "tcpserver.h"

#ifdef SACore
#include "application.h"
#endif

namespace SA
{
    struct TcpServer::TcpServerPrivate
    {
        SOCKET socketFd = INVALID_SOCKET;
        int mainLoopId = -1;
        bool isListen = false;
        bool isWinsockStarted = false;
        SOCKADDR_IN address;

        std::vector<SOCKET> sockets;
        std::map<int, std::function<void (int, uint16_t, uint32_t)> > connectHandlers;
    };

    TcpServer::TcpServer():
        d(new TcpServerPrivate)
    {
        WSADATA wsa;
        long rc = WSAStartup(MAKEWORD(2, 2), &wsa);
        d->isWinsockStarted = (rc == 0);

        if (d->isWinsockStarted)
        {
#ifdef SACore
        d->mainLoopId = SA::Application::instance().addMainLoopListener(std::bind(&TcpServer::mainLoopHandler, this));
#endif
        }
    }

    SA::TcpServer::~TcpServer()
    {
#ifdef SACore
        SA::Application::instance().removeMainLoopListener(d->mainLoopId);
#endif
        deleteServer();
        WSACleanup();
        delete d;
    }

    bool TcpServer::listen(uint16_t port)
    {
        if (!createServer()) return false;

        d->address.sin_family = AF_INET;
        d->address.sin_port = htons(port);
        d->address.sin_addr.s_addr = htonl(INADDR_ANY);

        int state = ::bind(d->socketFd, (struct sockaddr *)&d->address, sizeof(d->address));

        if (state > -1)
            state = ::listen(d->socketFd, 255);

        d->isListen = (state > -1);

        return d->isListen;
    }

    void TcpServer::close()
    {
        deleteServer();
    }

    bool TcpServer::isListen()
    {
        return d->isListen;
    }

    int TcpServer::addConnectHandler(const std::function<void (int, uint32_t, uint16_t)> &func)
    {
        int id = static_cast<int>(d->connectHandlers.size());
        for (auto const& it : d->connectHandlers) if (it.first != ++id) break;
        d->connectHandlers.insert({id, func});
        return id;
    }

    void TcpServer::removeConnectHandler(int id)
    {
        auto it = d->connectHandlers.find(id);
        if (it != d->connectHandlers.end())
            d->connectHandlers.erase(it);
    }

    void TcpServer::mainLoopHandler()
    {
        if (!d->isListen) return;

        SOCKADDR_IN socketAddr;
        int sockaddrLen = sizeof(socketAddr);

        SOCKET newsockfd = ::accept(d->socketFd, (SOCKADDR *)&socketAddr, &sockaddrLen);

        if (newsockfd != SOCKET_ERROR)
        {
            d->sockets.push_back(newsockfd);

            for (const auto &it: d->connectHandlers)
                it.second(static_cast<int>(newsockfd), socketAddr.sin_addr.s_addr, socketAddr.sin_port);
        }
    }

    bool TcpServer::createServer()
    {
        d->isListen = false;
        d->socketFd = socket(AF_INET, SOCK_STREAM, 0);
        bool isSocketCreated = (d->socketFd != INVALID_SOCKET);

        if (isSocketCreated) {

            int iSetOption = 0;
            setsockopt(d->socketFd, SOL_SOCKET, SO_REUSEADDR, (char*)&iSetOption, sizeof(iSetOption));

            int iVal = 10;
            setsockopt(d->socketFd, SOL_SOCKET, SO_RCVTIMEO, (char *)&iVal, sizeof(iVal));

            unsigned long iMode = 1;
            int res = ioctlsocket(d->socketFd, FIONBIO, &iMode);
            isSocketCreated = (res == NO_ERROR);
        }

        return isSocketCreated;
    }

    void TcpServer::deleteServer()
    {
        d->isListen = false;

        for (int socketFd : d->sockets) {
            if (socketFd != INVALID_SOCKET) {
                ::shutdown(socketFd, SD_BOTH);
                ::closesocket(socketFd);
            }
        }

        if (d->socketFd != INVALID_SOCKET) {
            ::shutdown(d->socketFd, SD_BOTH);
            ::closesocket(d->socketFd);
        }

        d->socketFd = INVALID_SOCKET;
    }
}

#endif //WIN32
