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

        void move(int x, int y);
        void resize(int width, int height);
        void setGeometry(int x, int y, int w, int h);

        int x();
        int y();
        int width();
        int height();

        void setPen(unsigned char red, unsigned char green,
                    unsigned char blue, unsigned int width);

        void setBrush(unsigned char red, unsigned char green,
                      unsigned char blue);

        void drawLine(int x1, int y1, int x2, int y2);
        void drawRect(int x, int y, int width, int height);
        void drawText(int x, int y, const std::string &text);

        int textWidth(const std::string &text);
        int textHeight();

        bool isHovered();

        virtual void keyPressEvent(bool state, unsigned int keyCode);
        virtual void paintEvent();
        virtual void moveEvent(int x, int y);
        virtual void resizeEvent(int width, int height);
        virtual void mouseMoveEvent(int x, int y);
        virtual void mouseHoverEvent(bool state);
        virtual void mousePressEvent(bool state, unsigned int button);

    private:
        void mainLoopEvent();
        void event(EventTypes type, const std::any &value);

        struct WidgetPrivate;
        WidgetPrivate * const d;

    }; // class Widget

} // namespace SA
