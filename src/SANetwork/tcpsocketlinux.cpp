#ifdef __linux__

#include <memory>
#include <vector>
#include <string>
#include <map>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "tcpsocket.h"

#ifdef SACore
#include "application.h"
#endif

static const size_t DefaultLen = 1024;
static const int ConnectionCheckInterval = 500;

namespace SA
{
    struct TcpSocket::TcpSocketPrivate
    {
        int socketFd = -1;
        int mainLoopId = -1;
        bool isConnected = false;
        sockaddr_in address;

        std::vector<char> dataIn, dataTmp;
        std::map<int, std::function<void (const std::vector<char>&)> > readHandlers;
        std::map<int, std::function<void (int)> > disconnectHandlers;
    };

    TcpSocket::TcpSocket():
        d(new TcpSocketPrivate)
    {
        d->dataIn.resize(DefaultLen, 0);
        d->dataTmp.reserve(DefaultLen);

#ifdef SACore
        d->mainLoopId = SA::Application::instance().addMainLoopListener(std::bind(&TcpSocket::mainLoopHandler, this));
#endif
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

        int state = ::connect(d->socketFd, (struct sockaddr *)&d->address, sizeof(d->address));
        d->isConnected = (state > -1);

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
        return (::send(d->socketFd, data.data(), data.size(), 0) > -1);
    }

    int TcpSocket::descriptor()
    {
        return d->socketFd;
    }

    void TcpSocket::setDescriptor(int descr)
    {
        if (descr <= -1) return;

        d->socketFd = descr;

        int optval = -1;
        socklen_t optlen = sizeof(optval);
        int res = getsockopt(descr, SOL_SOCKET, SO_ERROR, &optval, &optlen);
        d->isConnected = (optval==0 && res==0);

        struct timeval read_timeout;
        read_timeout.tv_sec = 0;
        read_timeout.tv_usec = 10;
        ::setsockopt(descr, SOL_SOCKET, SO_RCVTIMEO, &read_timeout, sizeof(read_timeout));
    }

    int TcpSocket::addReadHandler(const std::function<void (const std::vector<char>&)> &func)
    {
        int id = static_cast<int>(d->readHandlers.size());
        for (auto const& it : d->readHandlers) if (it.first != ++id) break;
        d->readHandlers.insert({id, func});
        return id;
    }

    void TcpSocket::removeReadHandler(int id)
    {
        auto it = d->readHandlers.find(id);
        if (it != d->readHandlers.end())
            d->readHandlers.erase(it);
    }

    int TcpSocket::addDisconnectHandler(const std::function<void (int)> &func)
    {
        int id = static_cast<int>(d->disconnectHandlers.size());
        for (auto const& it : d->disconnectHandlers) if (it.first != ++id) break;
        d->disconnectHandlers.insert({id, func});
        return id;
    }

    void TcpSocket::removeDisconnectHandler(int id)
    {
        auto it = d->disconnectHandlers.find(id);
        if (it != d->disconnectHandlers.end())
            d->disconnectHandlers.erase(it);
    }

    void TcpSocket::mainLoopHandler()
    {
        if (!d->isConnected) return;

        ssize_t bytesRead = ::recv(d->socketFd, d->dataIn.data(), d->dataIn.size(), 0);

        if (bytesRead > 0)
        {
            d->dataTmp.clear();
            d->dataTmp.insert(d->dataTmp.begin(), d->dataIn.begin(), d->dataIn.begin() + bytesRead);

            for (const auto &it: d->readHandlers)
                it.second(d->dataTmp);
        }
        else if(bytesRead == 0)
        {
            deleteSocket();

            for (const auto &it: d->disconnectHandlers)
                it.second(d->socketFd);
        }
    }

    bool TcpSocket::createSocket()
    {
        d->isConnected = false;
        int descr = socket(AF_INET, SOCK_STREAM, 0);
        setDescriptor(descr);
        return (descr > -1);
    }

    void TcpSocket::deleteSocket()
    {
        if (d->isConnected)
            ::close(d->socketFd);

        d->isConnected = false;
    }
}

#endif //__linux__
