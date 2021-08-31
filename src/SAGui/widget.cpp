#include <memory>
#include <iostream>

#include "SAGui/widget.h"
#include "SACore/application.h"

#ifdef __linux__
#include "SAGui/widgetlinux.h"
#endif //__linux__

#ifdef WIN32
#include "SAGui/widgetwindows.h"
#endif //WIN32

namespace SA
{
    struct Widget::WidgetPrivate
    {
        friend class SA::Widget;
        SA::Widget *parent = nullptr;

#ifdef __linux__
        SA::WidgetLinux *widget = nullptr;
#endif //__linux__

#ifdef WIN32
        SA::WidgetWindows *widget = nullptr;
#endif //WIN32

    };

    Widget::Widget(Widget *parent) : SA::Object(),
        d(new WidgetPrivate)
    {
        d->parent = parent;

#ifdef __linux__
        SA::WidgetLinux *parentWidget = d->parent ? d->parent->d->widget : nullptr;
        d->widget = new WidgetLinux(parentWidget);
        d->widget->addEventListener(this);
#endif //__linux__

#ifdef WIN32
        SA::WidgetWindows *parentWidget = d->parent ? d->parent->d->widget : nullptr;
        d->widget = new WidgetWindows(parentWidget);
        d->widget->addEventListener(this);
#endif //WIN32

        SA::Application::instance().addToMainLoop(this);
    }

    Widget::~Widget()
    {
        delete d->widget;
        delete d;
    }

    void Widget::show()
    {
        d->widget->show();
    }

    void Widget::hide()
    {
        d->widget->hide();
    }

    void Widget::update()
    {
        d->widget->update();
    }

    void Widget::setTitle(const std::string &title)
    {
        d->widget->setTitle(title);
    }

    void Widget::move(int x, int y)
    {
        d->widget->move(x, y);
    }

    void Widget::resize(int width, int height)
    {
        d->widget->resize(width, height);
    }

    void Widget::setGeometry(int x, int y, int w, int h)
    {
        d->widget->setGeometry(x, y, w, h);
    }

    int Widget::x()
    {
        return d->widget->x();
    }

    int Widget::y()
    {
        return d->widget->y();
    }

    int Widget::width()
    {
        return d->widget->width();
    }

    int Widget::height()
    {
        return d->widget->height();
    }

    void Widget::setPen(unsigned char red, unsigned char green,
                           unsigned char blue, unsigned int width)
    {
        d->widget->setPen(red, green, blue, width);
    }

    void Widget::setBrush(unsigned char red, unsigned char green,
                          unsigned char blue)
    {
        d->widget->setBrush(red, green, blue);
    }

    void Widget::drawLine(int x1, int y1, int x2, int y2)
    {
        d->widget->drawLine(x1, y1, x2, y2);
    }

    void Widget::drawRect(int x, int y, int width, int height)
    {
        d->widget->drawRect(x, y, width, height);
    }

    void Widget::drawText(int x, int y, const std::string &text)
    {
        d->widget->drawText(x, y, text);
    }

    int Widget::textWidth(const std::string &text)
    {
        return d->widget->textWidth(text);
    }

    int Widget::textHeight()
    {
        return d->widget->textHeight();
    }

    bool Widget::isHovered()
    {
        return d->widget->isHovered();
    }

    void Widget::keyPressEvent(bool state, unsigned int keyCode)
    {
        std::ignore = state;
        std::ignore = keyCode;
    }

    void Widget::paintEvent()
    {
    }

    void Widget::moveEvent(int x, int y)
    {
        std::ignore = x;
        std::ignore = y;
    }

    void Widget::resizeEvent(int width, int height)
    {
        std::ignore = width;
        std::ignore = height;
    }

    void Widget::mouseMoveEvent(int x, int y)
    {
        std::ignore = x;
        std::ignore = y;
    }

    void Widget::mouseHoverEvent(bool state)
    {
        std::ignore = state;
    }

    void Widget::mousePressEvent(bool state, unsigned int button)
    {
        std::ignore = state;
        std::ignore = button;
    }

    void Widget::focusEvent(bool state)
    {
        std::ignore = state;
    }

    void Widget::mainLoopEvent()
    {
        d->widget->mainLoopEvent();
    }

    void Widget::event(EventTypes type, const std::any &value)
    {
        switch (type)
        {
        case ButtonPressEvent:
            keyPressEvent(true, std::any_cast<unsigned int>(value));
            break;
        case EventTypes::ButtonReleaseEvent:
            keyPressEvent(false, std::any_cast<unsigned int>(value));
            break;
        case EventTypes::MouseMoveEvent:
        {
            auto mousePair = std::any_cast<std::pair<int, int> >(value);
            mouseMoveEvent(mousePair.first, mousePair.second);
            break;
        }
        case EventTypes::MouseHoverEvent:
        {
            mouseHoverEvent(std::any_cast<bool>(value));
            break;
        }
        case EventTypes::MousePressEvent:
        {
            mousePressEvent(true, std::any_cast<unsigned int>(value));
            break;
        }
        case EventTypes::MouseReleaseEvent:
        {
            mousePressEvent(false, std::any_cast<unsigned int>(value));
            break;
        }
        case EventTypes::PaintEvent:
            paintEvent();
            break;
        case EventTypes::MoveEvent:
        {
            auto movePair = std::any_cast<std::pair<int, int> >(value);
            moveEvent(movePair.first, movePair.second);
            break;
        }
        case EventTypes::FocusInEvent: focusEvent(true); break;
        case EventTypes::FocusOutEvent: focusEvent(false); break;
        case EventTypes::ResizeEvent:
        {
            auto sizePair = std::any_cast<std::pair<int, int> >(value);
            resizeEvent(sizePair.first, sizePair.second);
            break;
        }
        default:
            break;
        }
    }
}
