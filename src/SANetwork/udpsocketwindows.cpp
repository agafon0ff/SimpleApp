#ifdef WIN32

#include <winsock2.h>

#include <memory>
#include <vector>
#include <string>

#include "SANetwork/udpsocket.h"
#include "SACore/application.h"

static const size_t DefaultLen = 1024;

namespace SA
{
    struct UdpSocket::UdpSocketPrivate
    {
        SOCKET socketBind = INVALID_SOCKET;
        SOCKET socketSend = INVALID_SOCKET;
        bool isBinded = false;
        bool isWinsockStarted = false;
        SOCKADDR_IN addressSrc;

        std::vector<char> dataIn, dataTmp;
        std::map<int, std::function<void (const std::vector<char>&)> > readHanders;
    };

    UdpSocket::UdpSocket() : SA::Object(),
        d(new UdpSocketPrivate)
    {
        WSADATA wsa;
        long rc = WSAStartup(MAKEWORD(2, 0), &wsa);
        d->isWinsockStarted = (rc == 0);

        if (d->isWinsockStarted)
        {
            d->dataIn.resize(DefaultLen, 0);
            d->dataTmp.reserve(DefaultLen);
            d->socketSend = socket(AF_INET, SOCK_DGRAM, 0);

            SA::Application::instance().addToMainLoop(this);
        }
    }

    SA::UdpSocket::~UdpSocket()
    {
        deleteSocket();
        delete d;
    }

    bool UdpSocket::bind(uint16_t port)
    {
        return bind(static_cast<uint32_t>(INADDR_ANY), port);
    }

    bool UdpSocket::bind(uint32_t host, uint16_t port)
    {
        if (!createSocket()) return false;

        d->addressSrc.sin_family = AF_INET;
        d->addressSrc.sin_port = htons(port);
        d->addressSrc.sin_addr.s_addr = htonl(host);

        long state = ::bind(d->socketBind, (SOCKADDR *)&d->addressSrc, sizeof(d->addressSrc));
        d->isBinded = (state != SOCKET_ERROR);

        return d->isBinded;
    }

    bool UdpSocket::bind(const char* host, uint16_t port)
    {
        return bind(static_cast<uint32_t>(htonl(inet_addr(host))), port);
    }

    bool UdpSocket::bind(const std::string &host, uint16_t port)
    {
        return bind(host.c_str(), port);
    }

    bool UdpSocket::isBinded()
    {
        return d->isBinded;
    }

    void UdpSocket::unbind()
    {
        deleteSocket();
    }

    bool UdpSocket::send(const std::vector<char> &data, uint32_t host, uint16_t port)
    {
        if (!d->socketSend) return false;

        SOCKADDR_IN addr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        addr.sin_addr.s_addr = htonl(host);

        long state = sendto(d->socketSend, data.data(), data.size(), 0, (SOCKADDR*) &addr, sizeof(addr));
        return (state != SOCKET_ERROR);
    }

    bool UdpSocket::send(const std::vector<char> &data, const char *host, uint16_t port)
    {
        return send(data, htonl(inet_addr(host)), port);
    }

    bool UdpSocket::send(const std::vector<char> &data, const std::string &host, uint16_t port)
    {
        return send(data, host.c_str(), port);
    }

    int UdpSocket::addReadHandler(const std::function<void (const std::vector<char>&)> &func)
    {
        int id = 0;
        for (auto const& it : d->readHanders) if (it.first != ++id) break;
        d->readHanders.insert({id, func});
        return id;
    }

    void UdpSocket::removeReadHandler(int id)
    {
        auto it = d->readHanders.find(id);
        if (it != d->readHanders.end())
            d->readHanders.erase(it);
    }

    void UdpSocket::mainLoopEvent()
    {
        if (!d->isBinded) return;

        ssize_t bytesRead = recvfrom(d->socketBind, d->dataIn.data(),d->dataIn.size(), 0, (struct sockaddr*)NULL, NULL);

        if (bytesRead > -1)
        {
            d->dataTmp.clear();
            d->dataTmp.insert(d->dataTmp.begin(), d->dataIn.begin(), d->dataIn.begin() + bytesRead);

            for (const auto &it: d->readHanders)
                it.second(d->dataTmp);
        }
    }

    bool UdpSocket::createSocket()
    {
        d->isBinded = false;
        d->socketBind = socket(AF_INET, SOCK_DGRAM, 0);
        bool isSocketCreated = d->socketBind != INVALID_SOCKET;

        if (isSocketCreated)
        {
            int iVal = 10;
            setsockopt(d->socketBind, SOL_SOCKET, SO_RCVTIMEO, (char *)&iVal, sizeof(iVal));
        }

        return isSocketCreated;
    }

    void UdpSocket::deleteSocket()
    {
        d->isBinded = false;

        if (d->socketBind != INVALID_SOCKET)
            closesocket(d->socketBind);

        d->socketBind = INVALID_SOCKET;
    }
}

#endif //WIN32
