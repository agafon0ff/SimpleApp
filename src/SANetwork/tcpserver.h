#pragma once

#include <string>
#include <memory>
#include <functional>

#include "tcpsocket.h"

namespace SA
{
    class TcpServer
    {
    public:
        TcpServer();
        virtual ~TcpServer();

        bool listen(uint16_t port);
        void close();

        int addConnectHandler(const std::function<void (std::shared_ptr<TcpSocket>)> &func);
        void removeConnectHandler(int id);
        void mainLoopHandler();

    private:
        bool createSocket();
        void deleteSocket();

        struct TcpServerPrivate;
        TcpServerPrivate * const d;

    }; // class TcpServer
} // namespace SA
