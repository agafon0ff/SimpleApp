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

#include "tcpserver.h"

#ifdef SACore
#include "application.h"
#endif

static const size_t DefaultLen = 1024;

namespace SA
{
    struct TcpServer::TcpServerPrivate
    {
        int socket = -1;
        bool isConnected = false;
        sockaddr_in address;

        std::vector<char> dataIn, dataTmp;
        std::map<int, std::function<void (const std::vector<char>&)> > readHanders;
        std::map<int, std::function<void ()> > disconnectHanders;
    };

    TcpServer::TcpServer():
        d(new TcpServerPrivate)
    {
        d->dataIn.resize(DefaultLen, 0);
        d->dataTmp.reserve(DefaultLen);

#ifdef SACore
        SA::Application::instance().addMainLoopListener(std::bind(&TcpServer::mainLoopHandler, this));
#endif
    }

    SA::TcpServer::~TcpServer()
    {
        deleteSocket();
        delete d;
    }

    bool TcpServer::connect(uint32_t host, uint16_t port)
    {
        if (!createSocket()) return false;

        d->address.sin_family = AF_INET;
        d->address.sin_port = htons(port);
        d->address.sin_addr.s_addr = htonl(host);

        int state = ::connect(d->socket, (struct sockaddr *)&d->address, sizeof(d->address));
        d->isConnected = (state > -1);

        return d->isConnected;
    }

    bool TcpServer::connect(const char* host, uint16_t port)
    {
        return connect(htonl(inet_addr(host)), port);
    }

    bool TcpServer::connect(const std::string &host, uint16_t port)
    {
        return connect(host.c_str(), port);
    }

    bool TcpServer::isConnected()
    {
        return d->isConnected;
    }

    void TcpServer::disconnect()
    {
        deleteSocket();
    }

    bool TcpServer::send(const std::vector<char> &data)
    {
        if (!d->isConnected) return false;
        return (::send(d->socket, data.data(), data.size(), 0) > -1);
    }

    int TcpServer::addReadHandler(const std::function<void (const std::vector<char>&)> &func)
    {
        int id = static_cast<int>(d->readHanders.size());
        for (auto const& it : d->readHanders) if (it.first != ++id) break;
        d->readHanders.insert({id, func});
        return id;
    }

    void TcpServer::removeReadHandler(int id)
    {
        auto it = d->readHanders.find(id);
        if (it != d->readHanders.end())
            d->readHanders.erase(it);
    }

    int TcpServer::addDisconnectHandler(const std::function<void()> &func)
    {
        int id = static_cast<int>(d->disconnectHanders.size());
        for (auto const& it : d->disconnectHanders) if (it.first != ++id) break;
        d->disconnectHanders.insert({id, func});
        return id;
    }

    void TcpServer::removeDisconnectHandler(int id)
    {
        auto it = d->disconnectHanders.find(id);
        if (it != d->disconnectHanders.end())
            d->disconnectHanders.erase(it);
    }

    void TcpServer::mainLoopHandler()
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

    bool TcpServer::createSocket()
    {
        d->isConnected = false;
        d->socket = socket(AF_INET, SOCK_STREAM, 0);

        struct timeval read_timeout;
        read_timeout.tv_sec = 0;
        read_timeout.tv_usec = 10;
        setsockopt(d->socket, SOL_SOCKET, SO_RCVTIMEO, &read_timeout, sizeof(read_timeout));

        return (d->socket > -1);
    }

    void TcpServer::deleteSocket()
    {
        d->isConnected = false;

        if (d->socket > -1)
            ::close(d->socket);

        d->socket = -1;
    }
}

#endif //__linux__
