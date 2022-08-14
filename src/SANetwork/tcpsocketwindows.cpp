#ifdef WIN32

#include <winsock2.h>

#include <memory>
#include <vector>
#include <string>
#include <map>

#include "tcpsocket.h"

#ifdef SACore
#include "application.h"
#endif

static const size_t DefaultLen = 1024;

namespace SA
{
    struct TcpSocket::TcpSocketPrivate
    {
        int mainLoopId = -1;
        SOCKET socket = INVALID_SOCKET;
        bool isConnected = false;
        bool isWinsockStarted = false;
        SOCKADDR_IN address;

        std::vector<char> dataIn, dataTmp;
        std::map<int, std::function<void (const std::vector<char>&)> > readHanders;
        std::map<int, std::function<void ()> > disconnectHanders;
    };

    TcpSocket::TcpSocket():
        d(new TcpSocketPrivate)
    {
        WSADATA wsa;
        long rc = WSAStartup(MAKEWORD(2, 0), &wsa);
        d->isWinsockStarted = (rc == 0);

        if (d->isWinsockStarted)
        {
            d->dataIn.resize(DefaultLen, 0);
            d->dataTmp.reserve(DefaultLen);

#ifdef SACore
            d->mainLoopId = SA::Application::instance().addMainLoopListener(std::bind(&TcpSocket::mainLoopHandler, this));
#endif
        }
    }

    SA::TcpSocket::~TcpSocket()
    {
#ifdef SACore
        SA::Application::instance().removeMainLoopListener(d->mainLoopId);
#endif
        deleteSocket();
        delete d;
    }

    bool TcpSocket::connect(uint32_t host, uint16_t port)
    {
        if (!createSocket()) return false;

        d->address.sin_family = AF_INET;
        d->address.sin_port = htons(port);
        d->address.sin_addr.s_addr = htonl(host);

        int state = ::connect(d->socket, (SOCKADDR *)&d->address, sizeof(d->address));
        d->isConnected = (state > SOCKET_ERROR);

        return d->isConnected;
    }

    bool TcpSocket::connect(const char* host, uint16_t port)
    {
        return connect(htonl(inet_addr(host)), port);
    }

    bool TcpSocket::connect(const std::string &host, uint16_t port)
    {
        return connect(host.c_str(), port);
    }

    bool TcpSocket::isConnected()
    {
        return d->isConnected;
    }

    void TcpSocket::disconnect()
    {
        deleteSocket();
    }

    bool TcpSocket::send(const std::vector<char> &data)
    {
        if (!d->isConnected) return false;
        return (::send(d->socket, data.data(), data.size(), 0) > -1);
    }

    int TcpSocket::addReadHandler(const std::function<void (const std::vector<char>&)> &func)
    {
        int id = static_cast<int>(d->readHanders.size());
        for (auto const& it : d->readHanders) if (it.first != ++id) break;
        d->readHanders.insert({id, func});
        return id;
    }

    void TcpSocket::removeReadHandler(int id)
    {
        auto it = d->readHanders.find(id);
        if (it != d->readHanders.end())
            d->readHanders.erase(it);
    }

    int TcpSocket::addDisconnectHandler(const std::function<void()> &func)
    {
        int id = static_cast<int>(d->disconnectHanders.size());
        for (auto const& it : d->disconnectHanders) if (it.first != ++id) break;
        d->disconnectHanders.insert({id, func});
        return id;
    }

    void TcpSocket::removeDisconnectHandler(int id)
    {
        auto it = d->disconnectHanders.find(id);
        if (it != d->disconnectHanders.end())
            d->disconnectHanders.erase(it);
    }

    void TcpSocket::mainLoopHandler()
    {
        if (!d->isConnected) return;

        ssize_t bytesRead = ::recv(d->socket, d->dataIn.data(), d->dataIn.size(), 0);

        if (bytesRead > 0)
        {
            d->dataTmp.clear();
            d->dataTmp.insert(d->dataTmp.begin(), d->dataIn.begin(), d->dataIn.begin() + bytesRead);

            for (const auto &it: d->readHanders)
                it.second(d->dataTmp);
        }
        else if(bytesRead == 0)
        {
            deleteSocket();
            for (const auto &it: d->disconnectHanders)
                it.second();
        }
    }

    bool TcpSocket::createSocket()
    {
        d->isConnected = false;
        d->socket = socket(AF_INET, SOCK_STREAM, 0);
        bool isSocketCreated = d->socket != INVALID_SOCKET;

        if (isSocketCreated)
        {
            int iVal = 10;
            setsockopt(d->socket, SOL_SOCKET, SO_RCVTIMEO, (char *)&iVal, sizeof(iVal));
        }

        return isSocketCreated;
    }

    void TcpSocket::deleteSocket()
    {
        d->isConnected = false;

        if (d->socket != INVALID_SOCKET)
            closesocket(d->socket);

        d->socket = INVALID_SOCKET;
    }
}

#endif //WIN32
