#include <memory>
#include <iostream>

#include "SAGui/widget.h"
#include "SACore/global.h"
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

    void Widget::move(const Point &pos)
    {
        d->widget->move(pos.x, pos.y);
    }

    void Widget::move(int x, int y)
    {
        d->widget->move(x, y);
    }

    void Widget::resize(const Size &size)
    {
        d->widget->resize(size.width, size.height);
    }

    void Widget::resize(int width, int height)
    {
        d->widget->resize(width, height);
    }

    void Widget::setGeometry(const Rect &rect)
    {
        d->widget->setGeometry(rect.x, rect.y, rect.width, rect.height);
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

    Point Widget::pos()
    {
        return Point(d->widget->x(), d->widget->y());
    }

    Size Widget::size()
    {
        return Size(d->widget->width(), d->widget->height());
    }

    Rect Widget::geometry()
    {
        return Rect(d->widget->x(), d->widget->y(),
                    d->widget->width(), d->widget->height());
    }

    void Widget::setPen(const Pen &pen)
    {
        d->widget->setPen(pen.color.red, pen.color.green, pen.color.blue, pen.width);
    }

    void Widget::setPen(const Color &color, unsigned int width)
    {
        d->widget->setPen(color.red, color.green, color.blue, width);
    }

    void Widget::setPen(unsigned char red, unsigned char green,
                           unsigned char blue, unsigned int width)
    {
        d->widget->setPen(red, green, blue, width);
    }

    void Widget::setBrush(const Color &brush)
    {
        d->widget->setBrush(brush.red, brush.green, brush.blue);
    }

    void Widget::setBrush(unsigned char red, unsigned char green,
                          unsigned char blue)
    {
        d->widget->setBrush(red, green, blue);
    }

    void Widget::setCursorShape(CursorShapes shape)
    {
        d->widget->setCursorShape(shape);
    }

    void Widget::drawLine(const Point &p1, const Point &p2)
    {
        d->widget->drawLine(p1.x, p1.y, p2.x, p2.y);
    }

    void Widget::drawLine(int x1, int y1, int x2, int y2)
    {
        d->widget->drawLine(x1, y1, x2, y2);
    }

    void Widget::drawRect(const Rect &rect)
    {
        d->widget->drawRect(rect.x, rect.y, rect.width, rect.height);
    }

    void Widget::drawRect(int x, int y, int width, int height)
    {
        d->widget->drawRect(x, y, width, height);
    }

    void Widget::drawText(const Point &pos, const std::string &text)
    {
        d->widget->drawText(pos.x, pos.y, text);
    }

    void Widget::drawText(int x, int y, const std::string &text)
    {
        d->widget->drawText(x, y, text);
    }

    size_t Widget::textWidth(const std::string &text)
    {
        return d->widget->textWidth(text);
    }

    size_t Widget::textWidth(const char *text, size_t len)
    {
        return d->widget->textWidth(text, len);
    }

    size_t Widget::textHeight()
    {
        return d->widget->textHeight();
    }

    bool Widget::isHidden()
    {
        return d->widget->isHidden();
    }

    bool Widget::isHovered()
    {
        return d->widget->isHovered();
    }

    void Widget::keyboardEvent(const KeyEvent &event)
    {
        std::ignore = event;
    }

    void Widget::paintEvent()
    {
    }

    void Widget::moveEvent(const Point &pos)
    {
        std::ignore = pos;
    }

    void Widget::resizeEvent(const Size &size)
    {
        std::ignore = size;
    }

    void Widget::mouseMoveEvent(const Point &pos)
    {
        std::ignore = pos;
    }

    void Widget::mouseHoverEvent(bool state)
    {
        std::ignore = state;
    }

    void Widget::mouseButtonEvent(const MouseEvent &event)
    {
        std::ignore = event;
    }

    void Widget::mouseWheelEvent(int32_t delta)
    {
        std::ignore = delta;
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
        case KeyboardEvent:
            keyboardEvent(std::any_cast<KeyEvent>(value));
            break;
        case MouseMoveEvent:
        {
            auto mousePair = std::any_cast<std::pair<int32_t, int32_t> >(value);
            mouseMoveEvent({mousePair.first, mousePair.second});
            break;
        }
        case MouseHoverEvent:
        {
            mouseHoverEvent(std::any_cast<bool>(value));
            break;
        }
        case MouseButtonEvent:
        {
            mouseButtonEvent(std::any_cast<MouseEvent>(value));
            break;
        }
        case MouseWheelEvent:
        {
            mouseWheelEvent(std::any_cast<int32_t>(value));
            break;
        }
        case PaintEvent:
            paintEvent();
            break;
        case MoveEvent:
        {
            auto movePair = std::any_cast<std::pair<int32_t, int32_t> >(value);
            moveEvent({movePair.first, movePair.second});
            break;
        }
        case FocusInEvent: focusEvent(true); break;
        case FocusOutEvent: focusEvent(false); break;
        case ResizeEvent:
        {
            auto sizePair = std::any_cast<std::pair<uint32_t, uint32_t> >(value);
            resizeEvent({sizePair.first, sizePair.second});
            break;
        }
        default:
            break;
        }
    }
}
