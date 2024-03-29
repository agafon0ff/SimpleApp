#include <tuple>

#include "application.h"
#include "object.h"

namespace SA
{
    struct Object::ObjectPrivate
    {
    };

    Object::Object() :
        d(new ObjectPrivate)
    {
        SA::Application::instance().addMainLoopListener(this);
    }

    Object::~Object()
    {
        Application::instance().killTimers(this);
        Application::instance().removeMainLoopListener(this);
        delete d;
    }

    void Object::mainLoopEvent()
    {
    }

    void Object::event(EventTypes type, const std::any &value)
    {
        std::ignore = type;
        std::ignore = value;
    }

    void Object::timerEvent(int id)
    {
        std::ignore = id;
    }

    int Object::startTimer(int interval)
    {
        return Application::instance().startTimer(this, interval);
    }

    bool Object::killTimer(int id)
    {
        return Application::instance().killTimer(id);
    }
}
