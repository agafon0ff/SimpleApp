#pragma once

#include <string>
#include <functional>

#include "SACore/object.h"

namespace SA
{
    class TcpSocket : public SA::Object
    {
    public:
        TcpSocket();
        virtual ~TcpSocket();

        bool connect(uint32_t host, uint16_t port);
        bool connect(const char* host, uint16_t port);
        bool connect(const std::string &host, uint16_t port);

        bool isConnected();
        void disconnect();

        bool send(const std::vector<char> &data);

        int addReadHandler(const std::function<void (const std::vector<char> &)> &func);
        void removeReadHandler(int id);

        int addDisconnectHandler(const std::function<void ()> &func);
        void removeDisconnectHandler(int id);

    private:
        void mainLoopEvent();
        bool createSocket();
        void deleteSocket();

        struct TcpSocketPrivate;
        TcpSocketPrivate * const d;

    }; // class TcpSocket
} // namespace SA
