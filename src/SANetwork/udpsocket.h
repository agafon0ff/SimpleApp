#pragma once

#include <string>
#include <functional>

namespace SA
{
    class UdpSocket
    {
    public:
        UdpSocket();
        virtual ~UdpSocket();

        bool bind(uint16_t port); //Any
        bool bind(uint32_t host, uint16_t port);
        bool bind(const char* host, uint16_t port);
        bool bind(const std::string &host, uint16_t port);

        bool isBinded();

        void unbind();

        bool send(const std::vector<char> &data, uint32_t host, uint16_t port);
        bool send(const std::vector<char> &data, const char* host, uint16_t port);
        bool send(const std::vector<char> &data, const std::string &host, uint16_t port);

        int addReadHandler(const std::function<void (const std::vector<char> &)> &func);
        void removeReadHandler(int id);
        void mainLoopHandler();

    private:
        bool createSocket();
        void deleteSocket();

        UdpSocket(const SA::UdpSocket &) = delete;
        UdpSocket(SA::UdpSocket &&) = delete;
        void operator = (const SA::UdpSocket &) = delete;
        void operator = (SA::UdpSocket &&) = delete;

        struct UdpSocketPrivate;
        UdpSocketPrivate * const d;

    }; // class UdpSocket
} // namespace SA
