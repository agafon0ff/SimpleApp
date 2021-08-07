#include <tuple>

#include "SACore/application.h"
#include "SACore/object.h"

namespace SA
{
    struct Object::ObjectPrivate
    {
    };

    Object::Object() :
        d(new ObjectPrivate)
    {
    }

    Object::~Object()
    {
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
