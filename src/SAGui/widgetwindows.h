#pragma once

#ifdef WIN32
#include <windows.h>
#include <cstdint>
#include <string>
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

        void move(int32_t x, int32_t y);
        void resize(uint32_t width, uint32_t height);
        void setGeometry(int32_t x, int32_t y, uint32_t w, uint32_t h);

        int32_t x();
        int32_t y();
        uint32_t width();
        uint32_t height();

        void setPen(uint8_t red, uint8_t green, uint8_t blue, uint32_t width);
        void setBrush(uint8_t red, uint8_t green, uint8_t blue);
        void setCursorShape(CursorShapes shape);
        void setFont();

        void drawLine(int32_t x1, int32_t y1, int32_t x2, int32_t y2);
        void drawRect(int32_t x, int32_t y, uint32_t width, uint32_t height);
        void drawText(int32_t x, int32_t y, const std::string &text);

        size_t textWidth(const std::string &text);
        size_t textWidth(const char *text, size_t len);
        size_t textHeight();

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
