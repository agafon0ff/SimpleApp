#pragma once

#include <string>

#ifdef __linux__
#include <X11/Xlib.h>
#endif

#ifdef WIN32
#include <windows.h>
#endif // #ifdef WIN32

namespace SA
{
    class Clipboard
    {
    public:
        static Clipboard &instance();

        std::string getText();
        void setText(const std::string &text);

#ifdef __linux__
        void setNativePointers(Display *display, Window window);
        void onSelectionRequestEvent(XEvent *event);
#endif

#ifdef WIN32
    void setNativePointers(HWND hwnd);
#endif // #ifdef WIN32


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
