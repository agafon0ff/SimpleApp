#include <memory>
#include <vector>
#include <string>

#include "SANetwork/udpsocket.h"
#include "SACore/application.h"

#ifdef __linux__
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#endif //__linux__

#ifdef WIN32

#endif //WIN32

static const size_t DefaultLen = 1024;

namespace SA
{
    struct UdpSocket::UdpSocketPrivate
    {
        int socketBind = -1;
        int socketSend = -1;
        bool isBinded = false;

        sockaddr_in addressSrc;
        sockaddr_in addressDst;

        std::vector<char> dataIn, dataTmp;

        std::map<int, std::function<void (const std::vector<char>&)> > readHanders;
    };

    UdpSocket::UdpSocket() : SA::Object(),
        d(new UdpSocketPrivate)
    {
        d->dataIn.resize(DefaultLen, 0);
        d->dataTmp.reserve(DefaultLen);
        d->socketSend = socket(AF_INET, SOCK_DGRAM, 0);

        SA::Application::instance().addToMainLoop(this);
    }

    SA::UdpSocket::~UdpSocket()
    {
        deleteSocket();
        delete d;
    }

    bool UdpSocket::bind(uint16_t port)
    {
        return bind(INADDR_ANY, port);
    }

    bool UdpSocket::bind(uint32_t host, uint16_t port)
    {
        if (!createSocket()) return false;

        d->addressSrc.sin_family = AF_INET;
        d->addressSrc.sin_port = htons(port);
        d->addressSrc.sin_addr.s_addr = htonl(host);

        int state = ::bind(d->socketBind, (struct sockaddr *)&d->addressSrc, sizeof(d->addressSrc));
        d->isBinded = (state > -1);

        return d->isBinded;
    }

    bool UdpSocket::bind(const char* host, uint16_t port)
    {
        return bind(htonl(inet_addr(host)), port);
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

        struct sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        addr.sin_addr.s_addr = htonl(host);

        int state = sendto(d->socketSend, data.data(), data.size(), MSG_CONFIRM, (const struct sockaddr *) &addr, sizeof(addr));
        return (state > -1);
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

        struct timeval read_timeout;
        read_timeout.tv_sec = 0;
        read_timeout.tv_usec = 10;
        setsockopt(d->socketBind, SOL_SOCKET, SO_RCVTIMEO, &read_timeout, sizeof(read_timeout));

        return (d->socketBind > -1);
    }

    void UdpSocket::deleteSocket()
    {
        d->isBinded = false;

        if (d->socketBind > -1)
            ::close(d->socketBind);

        d->socketBind = -1;
    }
}
