#ifdef __linux__

#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <map>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "tcpserver.h"

#ifdef SACore
#include "application.h"
#endif

namespace SA
{
    struct TcpServer::TcpServerPrivate
    {
        int socketFd = -1;
        int mainLoopId = -1;
        bool isListen = false;
        sockaddr_in address;

        std::map<int, std::function<void (int, uint16_t, uint32_t)> > connectHandlers;
    };

    TcpServer::TcpServer():
        d(new TcpServerPrivate)
    {
#ifdef SACore
        d->mainLoopId = SA::Application::instance().addMainLoopListener(std::bind(&TcpServer::mainLoopHandler, this));
#endif
    }

    SA::TcpServer::~TcpServer()
    {
#ifdef SACore
        SA::Application::instance().removeMainLoopListener(d->mainLoopId);
#endif
        deleteServer();
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

        struct sockaddr_in socketAddr;
        socklen_t adrlen = sizeof(socketAddr);
        int newsockfd = accept(d->socketFd, (struct sockaddr *) &socketAddr, &adrlen);

        if (newsockfd > -1)
        {
            for (const auto &it: d->connectHandlers)
                it.second(d->socketFd, socketAddr.sin_addr.s_addr, socketAddr.sin_port);
        }
    }

    bool TcpServer::createServer()
    {
        d->isListen = false;
        d->socketFd = socket(AF_INET, SOCK_STREAM, 0);

        struct timeval read_timeout;
        read_timeout.tv_sec = 0;
        read_timeout.tv_usec = 10;
        setsockopt(d->socketFd, SOL_SOCKET, SO_RCVTIMEO, &read_timeout, sizeof(read_timeout));

        return (d->socketFd > -1);
    }

    void TcpServer::deleteServer()
    {
        d->isListen = false;

        if (d->socketFd > -1)
            ::close(d->socketFd);

        d->socketFd = -1;
    }

}

#endif //__linux__
