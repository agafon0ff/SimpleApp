#include <memory>
#include <iostream>

#include "widget.h"
#include "global.h"

#ifdef __linux__
#include "widgetlinux.h"
#endif //__linux__

#ifdef WIN32
#include "widgetwindows.h"
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
#endif //__linux__

#ifdef WIN32
        SA::WidgetWindows *parentWidget = d->parent ? d->parent->d->widget : nullptr;
        d->widget = new WidgetWindows(parentWidget);
#endif //WIN32

        d->widget->addEventListener(this);
    }

    Widget::~Widget()
    {
        d->widget->removeEventListener(this);
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

    void Widget::setIcon(const std::vector<uint8_t> &pixmap, size_t width, size_t height)
    {
        d->widget->setIcon(pixmap, width, height);
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

    SA::Point Widget::pos()
    {
        return SA::Point(d->widget->x(), d->widget->y());
    }

    SA::Size Widget::size()
    {
        return SA::Size(d->widget->width(), d->widget->height());
    }

    SA::Rect Widget::geometry()
    {
        return SA::Rect(d->widget->x(), d->widget->y(),
                    d->widget->width(), d->widget->height());
    }

    void Widget::setPen(const SA::Pen &pen)
    {
        d->widget->setPen(pen.color.red, pen.color.green, pen.color.blue, pen.width);
    }

    void Widget::setPen(const SA::Color &color, unsigned int width)
    {
        d->widget->setPen(color.red, color.green, color.blue, width);
    }

    void Widget::setPen(unsigned char red, unsigned char green,
                           unsigned char blue, unsigned int width)
    {
        d->widget->setPen(red, green, blue, width);
    }

    void Widget::setBrush(const SA::Color &brush)
    {
        d->widget->setBrush(brush.red, brush.green, brush.blue);
    }

    void Widget::setBrush(unsigned char red, unsigned char green,
                          unsigned char blue)
    {
        d->widget->setBrush(red, green, blue);
    }

    void Widget::setCursorShape(SA::CursorShapes shape)
    {
        d->widget->setCursorShape(shape);
    }

    SA::Point Widget::cursorPos()
    {
        return d->widget->cursorPos();
    }

    Size Widget::displaySize()
    {
        return d->widget->displaySize();
    }

    void Widget::drawLine(const SA::Point &p1, const SA::Point &p2)
    {
        d->widget->drawLine(p1.x, p1.y, p2.x, p2.y);
    }

    void Widget::drawLine(int x1, int y1, int x2, int y2)
    {
        d->widget->drawLine(x1, y1, x2, y2);
    }

    void Widget::drawRect(const SA::Rect &rect)
    {
        d->widget->drawRect(rect.x, rect.y, rect.width, rect.height);
    }

    void Widget::drawRect(int x, int y, int width, int height)
    {
        d->widget->drawRect(x, y, width, height);
    }

    void Widget::drawText(const SA::Point &pos, const std::string &text)
    {
        d->widget->drawText(pos.x, pos.y, text);
    }

    void Widget::drawText(int x, int y, const std::string &text)
    {
        d->widget->drawText(x, y, text);
    }

    void Widget::drawImage(const std::vector<uint8_t> &pixmap, const Rect &rect)
    {
        d->widget->drawImage(pixmap, rect);
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

    void Widget::keyboardEvent(const SA::KeyEvent &event)
    {
        std::ignore = event;
    }

    void Widget::paintEvent()
    {
    }

    void Widget::moveEvent(const SA::Point &pos)
    {
        std::ignore = pos;
    }

    void Widget::resizeEvent(const SA::Size &size)
    {
        std::ignore = size;
    }

    void Widget::mouseMoveEvent(const SA::Point &pos)
    {
        std::ignore = pos;
    }

    void Widget::mouseHoverEvent(bool state)
    {
        std::ignore = state;
    }

    void Widget::mouseButtonEvent(const SA::MouseEvent &event)
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

    void Widget::event(SA::EventTypes type, const std::any &value)
    {
        switch (type)
        {
        case SA::EventTypes::KeyboardEvent:
            keyboardEvent(std::any_cast<KeyEvent>(value));
            break;
        case SA::EventTypes::MouseMoveEvent:
        {
            auto mousePair = std::any_cast<std::pair<int32_t, int32_t> >(value);
            mouseMoveEvent({mousePair.first, mousePair.second});
            break;
        }
        case SA::EventTypes::MouseHoverEvent:
        {
            mouseHoverEvent(std::any_cast<bool>(value));
            break;
        }
        case SA::EventTypes::MouseButtonEvent:
        {
            mouseButtonEvent(std::any_cast<MouseEvent>(value));
            break;
        }
        case SA::EventTypes::MouseWheelEvent:
        {
            mouseWheelEvent(std::any_cast<int32_t>(value));
            break;
        }
        case SA::EventTypes::PaintEvent:
            paintEvent();
            break;
        case SA::EventTypes::MoveEvent:
        {
            auto movePair = std::any_cast<std::pair<int32_t, int32_t> >(value);
            moveEvent({movePair.first, movePair.second});
            break;
        }
        case SA::EventTypes::FocusInEvent: focusEvent(true); break;
        case SA::EventTypes::FocusOutEvent: focusEvent(false); break;
        case SA::EventTypes::ResizeEvent:
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
