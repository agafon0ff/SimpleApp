#pragma once

#ifdef __linux__
#include <string>
#include <memory>
#include <functional>
#include <X11/Xlib.h>

#include "SACore/object.h"

namespace SA
{
    class WidgetLinux
    {
    public:
        explicit WidgetLinux(WidgetLinux *parent = nullptr);
        virtual ~WidgetLinux();

        void show();
        void hide();
        void update();

        void setTitle(const std::string &title);

        void move(int x, int y);
        void resize(int width, int height);
        void setGeometry(int x, int y, int w, int h);

        int x();
        int y();
        int width();
        int height();

        void setPen(unsigned char red, unsigned char green,
                    unsigned char blue, unsigned int width);

        void setBrush(unsigned char red, unsigned char green, unsigned char blue);

        void setFont();

        void drawLine(int x1, int y1, int x2, int y2);
        void drawRect(int x, int y, int width, int height);
        void drawText(int x, int y, const std::string &text);
        void drawImage(int x, int y, int width, int height, const std::string &path);

        int textWidth(const std::string &text);
        int textHeight();

        bool isHovered();

        void mainLoopEvent();
        void addEventListener(SA::Object *object);

    private:

        void procEvent(_XEvent *event);
        void sendEvent(SA::EventTypes type, const std::any &value);
        void geometryUpdated();
        void setWindowProperties();

        struct WidgetLinuxPrivate;
        WidgetLinuxPrivate * const d;
    };
}
#endif

