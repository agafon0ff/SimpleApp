#pragma once

#ifdef WIN32
#include <string>
#include <windows.h>
#include <map>
#include "SACore/object.h"
#include "SACore/structs.h"

namespace SA
{
    class WidgetWindows
    {
    public:
        explicit WidgetWindows(WidgetWindows *parent = nullptr);
        virtual ~WidgetWindows();

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

        int textWidth(const std::string &text);
        int textHeight();

        bool isHovered();

        void mainLoopEvent();
        void addEventListener(SA::Object *object);

        int windowProc(unsigned int msg, unsigned int &wParam, long &lParam);
    private:

        void sendEvent(SA::EventTypes type, const std::any &value);
        void focusEvent(bool state);
        void keyEvent(unsigned int param, bool pressed);
        void mouseEvent(MouseButton btn, bool pressed);
        void geometryUpdated();

        struct WidgetWindowsPrivate;
        WidgetWindowsPrivate * const d;
    };
}

#endif //WIN32
