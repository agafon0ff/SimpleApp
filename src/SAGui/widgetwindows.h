#pragma once

#ifdef WIN32
#include <windows.h>
#include <cstdint>
#include <string>
#include <map>
#include <vector>

#include "object.h"
#include "structs.h"

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
        void setIcon(const std::vector<uint8_t> &pixmap, size_t width, size_t height);

        void move(int32_t x, int32_t y);
        void resize(uint32_t width, uint32_t height);
        void setGeometry(int32_t x, int32_t y, uint32_t w, uint32_t h);

        int32_t x();
        int32_t y();
        uint32_t width();
        uint32_t height();

        void setPen(uint8_t red, uint8_t green, uint8_t blue, uint32_t width);
        void setBrush(uint8_t red, uint8_t green, uint8_t blue);
        void setCursorShape(SA::CursorShapes shape);
        void setFont();

        SA::Point cursorPos();
        SA::Size displaySize();

        void drawLine(int32_t x1, int32_t y1, int32_t x2, int32_t y2);
        void drawRect(int32_t x, int32_t y, uint32_t width, uint32_t height);
        void drawText(int32_t x, int32_t y, const std::string &text);
        void drawImage(const std::vector<uint8_t> &pixmap, const SA::Rect &rect);

        size_t textWidth(const std::string &text);
        size_t textWidth(const char *text, size_t len);
        size_t textHeight();

        bool isHidden();
        bool isHovered();

        void mainLoopEvent();
        void addEventListener(SA::Object *object);
        void removeEventListener(SA::Object *object);

        int windowProc(unsigned int msg, unsigned int wParam, long lParam);

    private:
        void sendEvent(SA::EventTypes type, const std::any &value);
        void focusEvent(bool state);
        void keyEvent(unsigned int param, bool pressed);
        void mouseEvent(SA::MouseButton btn, bool pressed);
        void repaintWidget();
        void geometryUpdated();

        struct WidgetWindowsPrivate;
        WidgetWindowsPrivate * const d;
    };
}

#endif //WIN32
