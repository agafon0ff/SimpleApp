#pragma once

#include <cstddef>
#include <string>
#include <functional>
#include "object.h"

namespace SA
{
    class Application
    {
    public:
        static Application &instance();
        int exec();
        void quit(int exitCode = 0);

        int addMainLoopHandler(const std::function<void ()> &handler);
        void removeMainLoopHandler(int id);

        int startTimer(SA::Object *object, int interval);
        bool killTimer(int id);

    protected:
        Application();
        virtual ~Application();

    private:
        void timesStep();

        Application(const Application &in) = delete;
        Application(Application &&in) = delete;
        Application& operator=(const Application &in) = delete;
        void* operator new[](std::size_t) = delete;
        void operator delete[](void*) = delete;

        struct ApplicationPrivate;
        ApplicationPrivate * const d;

    }; // class Application

} // namespace SA

