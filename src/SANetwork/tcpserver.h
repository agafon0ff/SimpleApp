#pragma once

#include <string>
#include <memory>
#include <functional>

namespace SA
{
    class TcpServer
    {
    public:
        TcpServer();
        virtual ~TcpServer();

        bool listen(uint16_t port);
        void close();
        bool isListen();

        int addConnectHandler(const std::function<void (int sockDscr, uint32_t host, uint16_t port)> &func);
        void removeConnectHandler(int id);
        void mainLoopHandler();

    private:
        bool createServer();
        void deleteServer();

        TcpServer(const SA::TcpServer &) = delete;
        TcpServer(SA::TcpServer &&) = delete;
        void operator = (const SA::TcpServer &) = delete;
        void operator = (SA::TcpServer &&) = delete;

        struct TcpServerPrivate;
        TcpServerPrivate * const d;

    }; // class TcpServer
} // namespace SA
