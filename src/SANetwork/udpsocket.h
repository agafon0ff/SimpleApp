#pragma once

#include <string>
#include <functional>

#include "SACore/object.h"
#include "SACore/structs.h"

namespace SA
{
    class UdpSocket : public SA::Object
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

    private:
        void mainLoopEvent();
        bool createSocket();
        void deleteSocket();

        struct UdpSocketPrivate;
        UdpSocketPrivate * const d;

    }; // class Widget
} // namespace SA
