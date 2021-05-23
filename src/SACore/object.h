#pragma once

#include <any>
#include "SACore/global.h"

namespace SA
{
    class Object
    {
    public:
        Object();
        virtual ~Object();
        virtual void mainLoopEvent();
        virtual void event(SA::EventTypes type, const std::any &value);
        virtual void timerEvent(int id);

        int startTimer(int interval);
        bool killTimer(int id);

    private:
        struct ObjectPrivate;
        ObjectPrivate * const d;
    };
}
