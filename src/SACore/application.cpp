#include <algorithm>
#include <iostream>
#include <thread>
#include <chrono>
#include <ctime>
#include <list>
#include <map>

#include "application.h"

namespace SA {

    struct TimerStruct
    {
        SA::Object *object;
        int interval;
        bool removed = false;
        std::chrono::time_point<std::chrono::steady_clock> timeEnd;

        TimerStruct(SA::Object *_object, int _interval,
                    std::chrono::time_point<std::chrono::steady_clock> _timeEnd):
            object(_object), interval(_interval), timeEnd(_timeEnd){}
    };

    struct Application::ApplicationPrivate
    {
        int exitCode = 0;
        bool quitFlag = false;

        std::map<int, std::function<void ()> > mainLoopHandlers;
        std::map<int, TimerStruct> timers;
    };

    Application &Application::instance()
    {
        static Application * const ptr = new Application();
        return *ptr;
    }

    int Application::exec()
    {
        while(1)
        {
            timesStep();

            for (const auto &it: d->mainLoopHandlers)
                it.second();

            if (d->quitFlag) break;

            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }

        return d->exitCode;
    }

    void Application::quit(int exitCode)
    {
        d->exitCode = exitCode;
        d->quitFlag = true;
    }

    int Application::addMainLoopHandler(const std::function<void ()> &handler)
    {
        int id = static_cast<int>(d->mainLoopHandlers.size());
        for (auto const& it : d->mainLoopHandlers) if (it.first != ++id) break;
        d->mainLoopHandlers.insert({id, handler});

        return id;
    }

    void Application::removeMainLoopHandler(int id)
    {
        auto it = d->mainLoopHandlers.find(id);
        if (it != d->mainLoopHandlers.end())
            d->mainLoopHandlers.erase(it);
    }

    int Application::startTimer(Object *object, int interval)
    {
        if (!object) return -1;
        int newId = 1;

        for (const auto &timer : d->timers)
            if (timer.first == newId) ++newId;

        std::chrono::time_point<std::chrono::steady_clock> timeEnd;
        timeEnd = std::chrono::steady_clock::now() + std::chrono::milliseconds(interval);

        d->timers.insert({newId, TimerStruct(object, interval, timeEnd)});
        return newId;
    }

    bool Application::killTimer(int id)
    {
        auto it = d->timers.find(id);

        if (it == d->timers.end())
            return false;

        it->second.removed = true;
        return true;
    }

    Application::Application(): d(new ApplicationPrivate)
    {
    }

    Application::~Application()
    {
        delete d;
    }

    void Application::timesStep()
    {
        if (d->timers.size() == 0) return;

        auto &&now = std::chrono::steady_clock::now();
        std::map<int, TimerStruct>::iterator it, prev;
        prev = d->timers.end();

        for (it=d->timers.begin(); it!=d->timers.end(); ++it)
        {
            if (prev != d->timers.end())
                if (prev->second.removed)
                    d->timers.erase(prev);

            if (it->second.timeEnd <= now)
            {
                it->second.timeEnd = now + std::chrono::milliseconds(it->second.interval);
                it->second.object->timerEvent(it->first);
            }

            prev = it;
        }

        if (prev != d->timers.end())
            if (prev->second.removed)
                d->timers.erase(prev);
    }
}
