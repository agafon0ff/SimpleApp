#pragma once

#include <string>
#include <X11/Xlib.h>

namespace SA
{
    class Clipboard
    {
    public:
        static Clipboard &instance();

        std::string getText();
        void setText(const std::string &text);

        void setNativePointers(Display *display, Window window);
        void onSelectionRequestEvent(XEvent *event);

    protected:
        Clipboard ();
        virtual ~Clipboard();

    private:

        Clipboard(const Clipboard &in) = delete;
        Clipboard(Clipboard &&in) = delete;
        Clipboard& operator=(const Clipboard &in) = delete;
        void* operator new[](std::size_t) = delete;
        void operator delete[](void*) = delete;

        struct ClipboardPrivate;
        ClipboardPrivate * const d;

    }; // class Clipboard

} // namespace SA
