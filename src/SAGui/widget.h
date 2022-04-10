#pragma once
#include "SACore/object.h"
#include "SACore/structs.h"
#include <string>

namespace SA
{
    class Widget : public SA::Object
    {
    public:
        explicit Widget(Widget *parent = nullptr);
        virtual ~Widget();

        void show();
        void hide();
        void update();

        void setTitle(const std::string &title);

        void move(const SA::Point &pos);
        void move(int x, int y);

        void resize(const SA::Size &size);
        void resize(int width, int height);

        void setGeometry(const SA::Rect &rect);
        void setGeometry(int x, int y, int w, int h);

        int x();
        int y();
        int width();
        int height();

        SA::Point pos();
        SA::Size size();
        SA::Rect geometry();

        void setPen(const SA::Pen &pen);
        void setPen(const SA::Color &color, unsigned int width);
        void setPen(unsigned char red, unsigned char green,
                    unsigned char blue, unsigned int width);

        void setBrush(const SA::Color &brush);
        void setBrush(unsigned char red, unsigned char green,
                      unsigned char blue);

        void setCursorShape(SA::CursorShapes shape);

        void drawLine(const SA::Point &p1, const Point &p2);
        void drawLine(int x1, int y1, int x2, int y2);

        void drawRect(const SA::Rect &rect);
        void drawRect(int x, int y, int width, int height);

        void drawText(const SA::Point &pos, const std::string &text);
        void drawText(int x, int y, const std::string &text);

        size_t textWidth(const std::string &text);
        size_t textWidth(const char* text, size_t len);
        size_t textHeight();

        bool isHidden();
        bool isHovered();

    protected:
        virtual void keyboardEvent(const KeyEvent &event);
        virtual void paintEvent();
        virtual void moveEvent(const SA::Point &pos);
        virtual void resizeEvent(const SA::Size &size);
        virtual void mouseMoveEvent(const SA::Point &pos);
        virtual void mouseHoverEvent(bool state);
        virtual void mouseButtonEvent(const SA::MouseEvent &event);
        virtual void mouseWheelEvent(int32_t delta);
        virtual void focusEvent(bool state);

    private:
        void mainLoopEvent();
        void event(SA::EventTypes type, const std::any &value);

        struct WidgetPrivate;
        WidgetPrivate * const d;

    }; // class Widget

} // namespace SA
