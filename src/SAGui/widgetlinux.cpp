#ifdef __linux__

#include "SACore/application.h"
#include "SAGui/widgetlinux.h"
#include "SAGui/clipboard.h"

#include <X11/cursorfont.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/Xatom.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <map>

extern int errno;
using std::cout;
using std::endl;

namespace SA
{
    static const std::map<unsigned int, SA::Keys> KEYS_MAP = {
        { XK_Escape,        SA::Key_Escape      },
        { XK_Tab,           SA::Key_Tab         },
        { XK_BackSpace,     SA::Key_Backspace   },
        { XK_Return,        SA::Key_Return      },
        { XK_Insert,        SA::Key_Insert      },
        { XK_Delete,        SA::Key_Delete      },
        { XK_Pause,         SA::Key_Pause       },
        { XK_Print,         SA::Key_Print       },
        { XK_Clear,         SA::Key_Clear       },
        { XK_Home,          SA::Key_Home        },
        { XK_End,           SA::Key_End         },
        { XK_Left,          SA::Key_Left        },
        { XK_Up,            SA::Key_Up          },
        { XK_Right,         SA::Key_Right       },
        { XK_Down,          SA::Key_Down        },
        { XK_Page_Up,       SA::Key_PageUp      },
        { XK_Page_Down,     SA::Key_PageDown    },
        { XK_Shift_L,       SA::Key_LShift      },
        { XK_Shift_R,       SA::Key_RShift      },
        { XK_Control_R,     SA::Key_ControlR    },
        { XK_Control_L,     SA::Key_ControlL    },
        { XK_Alt_L,         SA::Key_AltL        },
        { XK_Alt_R,         SA::Key_AltR        },
        { XK_Caps_Lock,     SA::Key_CapsLock    },
        { XK_Num_Lock,      SA::Key_NumLock     },
        { XK_Scroll_Lock,   SA::Key_ScrollLock  },
        { XK_F1,            SA::Key_F1          },
        { XK_F2,            SA::Key_F2          },
        { XK_F3,            SA::Key_F3          },
        { XK_F4,            SA::Key_F4          },
        { XK_F5,            SA::Key_F5          },
        { XK_F6,            SA::Key_F6          },
        { XK_F7,            SA::Key_F7          },
        { XK_F8,            SA::Key_F8          },
        { XK_F9,            SA::Key_F9          },
        { XK_F10,           SA::Key_F10         },
        { XK_F11,           SA::Key_F11         },
        { XK_F12,           SA::Key_F12         },
        { XK_F13,           SA::Key_F13         },
        { XK_F14,           SA::Key_F14         },
        { XK_F15,           SA::Key_F15         },
        { XK_F16,           SA::Key_F16         },
        { XK_F17,           SA::Key_F17         },
        { XK_F18,           SA::Key_F18         },
        { XK_F19,           SA::Key_F19         },
        { XK_F20,           SA::Key_F20         },
        { XK_F21,           SA::Key_F21         },
        { XK_F22,           SA::Key_F22         },
        { XK_F23,           SA::Key_F23         },
        { XK_F24,           SA::Key_F24         },
        { XK_Menu,          SA::Key_Menu        },
        { XK_Help,          SA::Key_Help        },
        { XK_space,         SA::Key_Space       },
        { XK_quotedbl,      SA::Key_Quote       },
        { XK_asterisk,      SA::Key_Asterisk    },
        { XK_plus,          SA::Key_Plus        },
        { XK_comma,         SA::Key_Comma       },
        { XK_minus,         SA::Key_Minus       },
        { XK_equal,         SA::Key_Equal       },
        { XK_period,        SA::Key_Period      },
        { XK_slash,         SA::Key_Slash       },
        { XK_0,             SA::Key_0           },
        { XK_1,             SA::Key_1           },
        { XK_2,             SA::Key_2           },
        { XK_3,             SA::Key_3           },
        { XK_4,             SA::Key_4           },
        { XK_5,             SA::Key_5           },
        { XK_6,             SA::Key_6           },
        { XK_7,             SA::Key_7           },
        { XK_8,             SA::Key_8           },
        { XK_9,             SA::Key_9           },
        { XK_semicolon,     SA::Key_Semicolon   },
        { XK_a,             SA::Key_A           },
        { XK_b,             SA::Key_B           },
        { XK_c,             SA::Key_C           },
        { XK_d,             SA::Key_D           },
        { XK_e,             SA::Key_E           },
        { XK_f,             SA::Key_F           },
        { XK_g,             SA::Key_G           },
        { XK_h,             SA::Key_H           },
        { XK_i,             SA::Key_I           },
        { XK_j,             SA::Key_J           },
        { XK_k,             SA::Key_K           },
        { XK_l,             SA::Key_L           },
        { XK_m,             SA::Key_M           },
        { XK_n,             SA::Key_N           },
        { XK_o,             SA::Key_O           },
        { XK_p,             SA::Key_P           },
        { XK_q,             SA::Key_Q           },
        { XK_r,             SA::Key_R           },
        { XK_s,             SA::Key_S           },
        { XK_t,             SA::Key_T           },
        { XK_u,             SA::Key_U           },
        { XK_v,             SA::Key_V           },
        { XK_w,             SA::Key_W           },
        { XK_x,             SA::Key_X           },
        { XK_y,             SA::Key_Y           },
        { XK_z,             SA::Key_Z           },
        { XK_bracketleft,   SA::Key_BracketLeft },
        { XK_backslash,     SA::Key_Backslash   },
        { XK_bracketright,  SA::Key_BracketRight},
        { XK_asciitilde,    SA::Key_AsciiTilde  },
        { XK_quoteleft,     SA::Key_QuoteLeft   },
        { XK_quoteright,    SA::Key_QuoteRight  }
    //    { XK_Mute,       SA::Key_VolumeMute  },
    //    { XK_VOLUME_DOWN,       SA::Key_VolumeDown  },
    //    { XK_VOLUME_UP,         SA::Key_VolumeUp    },
    //    { XK_MEDIA_NEXT_TRACK,  SA::Key_MediaNext   },
    //    { XK_MEDIA_PREV_TRACK,  SA::Key_MediaPrev   },
    //    { XK_MEDIA_STOP,        SA::Key_MediaStop   },
    //    { XK_MEDIA_PLAY_PAUSE,  SA::Key_MediaPlay   }
    }; // KEYS_MAP

    static std::map<Window, WidgetLinux*> WIDGETS_MAP;
    static WidgetLinux* WIDGET_IN_FOCUS = nullptr;

    struct WidgetLinux::WidgetLinuxPrivate
    {
        friend class WidgetLinux;

        WidgetLinux *parent = nullptr;
        Display *display = nullptr;
        Window window;
        XEvent event;
        int screen;
        GC gc;
        XFontStruct *font = nullptr;

        fd_set inFileDescriptor;
        int x11FileDescriptor;

        int32_t x = 0;
        int32_t y = 0;
        uint32_t width = 200;
        uint32_t height = 200;
        bool isHidden = false;
        bool isPosChanged = false;
        bool isHovered = false;

        unsigned long widthPen = 1L;
        unsigned long colorPen = 0L;
        unsigned long colorBrush = 0L;

        std::vector<SA::Object*> eventListners;
    };

    WidgetLinux::WidgetLinux(WidgetLinux *parent):
        d(new WidgetLinuxPrivate)
    {
        d->parent = parent;

        if (!d->parent)
        {
            /* Connect to X-server */

            if (!d->display)
                d->display = XOpenDisplay(getenv("DISPLAY"));

            if (d->display == nullptr)
            {
                std::cout << "Can't connect X server: " << strerror(errno) << std::endl;
                Application::instance().quit(errno);
                return;
            }

            d->screen = XDefaultScreen(d->display);

            /* Create window */
            d->window = XCreateSimpleWindow(d->display,
                                            RootWindow(d->display, d->screen),
                                            d->x, d->y, d->width, d->height, 1,
                                            XBlackPixel(d->display, d->screen),
                                            XWhitePixel(d->display, d->screen));

            /* X-server messages */
            Atom WM_DELETE_WINDOW = XInternAtom(d->display, "WM_DELETE_WINDOW", False);
            XSetWMProtocols(d->display, d->window, &WM_DELETE_WINDOW, 1);
        }
        else
        {
            d->display = d->parent->d->display;
            d->screen = d->parent->d->screen;

            d->window = XCreateSimpleWindow(d->display,
                                            d->parent->d->window,
                                            d->x, d->y, d->width, d->height, 0,
                                            XBlackPixel(d->display, d->screen),
                                            XWhitePixel(d->display, d->screen));

            show();
        }

        /* What events need to be responded to */
        XSelectInput(d->display, d->window, ExposureMask |
                     KeyPressMask | KeyReleaseMask | FocusChangeMask |
                     ButtonPressMask | ButtonReleaseMask |
                     PointerMotionMask | StructureNotifyMask |
                     EnterWindowMask | LeaveWindowMask |
                     SubstructureNotifyMask | VisibilityChangeMask);

        d->gc = DefaultGC(d->display, d->screen);
        d->x11FileDescriptor = ConnectionNumber(d->display);

        setFont();

        if (WIDGETS_MAP.empty())
        {
            Clipboard &clipboard = Clipboard::instance();
            clipboard.setNativePointers(d->display, d->window);
        }

        WIDGETS_MAP.insert({d->window, this});


    }

    WidgetLinux::~WidgetLinux()
    {
        if (d->font)
            XFreeFont(d->display, d->font);

        WIDGETS_MAP.erase(WIDGETS_MAP.find(d->window));
        XDestroyWindow(d->display, d->window);

        if (!d->parent)
            XCloseDisplay(d->display);

        delete d;
    }

    void WidgetLinux::show()
    {
        d->isHidden = false;
        XMapWindow(d->display, d->window);

        if (d->isPosChanged)
        {
            XMoveWindow(d->display, d->window, d->x, d->y);
            d->isPosChanged = false;
        }

        XFlush(d->display);
        geometryUpdated();
    }

    void WidgetLinux::hide()
    {
        d->isHidden = true;
        XUnmapWindow(d->display, d->window);
        XFlush(d->display);
    }

    void WidgetLinux::update()
    {
        XEvent event = {0};
        event.type = Expose;
        event.xexpose.display = d->display;
        event.xexpose.window = d->window;
        XSendEvent(d->display, d->window, False, ExposureMask, &event);
    }

    void WidgetLinux::setTitle(const std::string &title)
    {
        XStoreName(d->display, d->window, title.c_str());
    }

    void WidgetLinux::move(int32_t x, int32_t y)
    {
        d->x = x;
        d->y = y;
        d->isPosChanged = true;
        XMoveWindow(d->display, d->window, d->x, d->y);
    }

    void WidgetLinux::resize(uint32_t width, uint32_t height)
    {
        d->width = width;
        d->height = height;
        XResizeWindow(d->display, d->window, d->width, d->height);
    }

    void WidgetLinux::setGeometry(int32_t x, int32_t y, uint32_t w, uint32_t h)
    {
        d->x = x;
        d->y = y;
        d->width = w;
        d->height = h;
        d->isPosChanged = true;
        XMoveResizeWindow(d->display, d->window, d->x, d->y, d->width, d->height);
    }

    int32_t WidgetLinux::x()
    {
        return d->x;
    }

    int32_t WidgetLinux::y()
    {
        return d->y;
    }

    uint32_t WidgetLinux::width()
    {
        return d->width;
    }

    uint32_t WidgetLinux::height()
    {
        return d->height;
    }

    void WidgetLinux::setPen(uint8_t red, uint8_t green, uint8_t blue, uint32_t width)
    {
        d->widthPen = width;
        d->colorPen = 0L;
        d->colorPen += red;
        d->colorPen <<= 8;
        d->colorPen += green;
        d->colorPen <<= 8;
        d->colorPen += blue;
    }

    void WidgetLinux::setBrush(uint8_t red, uint8_t green, uint8_t blue)
    {
        d->colorBrush = 0L;
        d->colorBrush += red;
        d->colorBrush <<= 8;
        d->colorBrush += green;
        d->colorBrush <<= 8;
        d->colorBrush += blue;
    }

    void WidgetLinux::setCursorShape(CursorShapes shape)
    {
        int shapeType = XC_arrow;
        switch (shape)
        {
        case Arrow: shapeType = XC_arrow; break;
        case Text: shapeType = XC_xterm; break;
        }

        XDefineCursor(d->display, d->window, XCreateFontCursor(d->display, shapeType));
    }

    void WidgetLinux::setFont()
    {
        if (d->font)
        {
            XFreeFont(d->display, d->font);
            d->font = nullptr;
        }

        // https://www.oreilly.com/library/view/x-window-system/9780937175149/Chapter05.html
        const char *fontname = "-misc-fixed-bold-r-normal--13-120-75-75-c-80-iso8859-1";
        d->font = XLoadQueryFont(d->display, fontname);
        /* If the font could not be loaded, revert to the "fixed" font. */
        if (!d->font)
        {
            std::cout << " error: unable to load font: " << fontname << ", using fixed" << std::endl;
            d->font = XLoadQueryFont(d->display, "fixed");
            std::cout << " Loaded font: " << d->font->properties->name << std::endl;
        }

        XSetFont(d->display, d->gc, d->font->fid);
    }

    void WidgetLinux::drawLine(int32_t x1, int32_t y1, int32_t x2, int32_t y2)
    {
        XGCValues gcv = {0};
        gcv.line_width = d->widthPen;
        XChangeGC(d->display, d->gc, GCLineWidth, &gcv);
        XSetForeground(d->display, d->gc, d->colorPen);

        XDrawLine(d->display,
                  d->window,
                  d->gc,
                  x1, y1, x2, y2);
    }

    void WidgetLinux::drawRect(int32_t x, int32_t y, uint32_t width, uint32_t height)
    {
        XSetForeground(d->display, d->gc, d->colorBrush);

        XFillRectangle(d->display,
                       d->window,
                       d->gc,
                       x, y, width, height);

        XGCValues gcv = {0};
        gcv.line_width = d->widthPen;
        XChangeGC(d->display, d->gc, GCLineWidth, &gcv);
        XSetForeground(d->display, d->gc, d->colorPen);

        XDrawRectangle(d->display,
                       d->window,
                       d->gc,
                       x, y, width, height);

//        GXcopyInverted();
    }

    void WidgetLinux::drawText(int32_t x, int32_t y, const std::string &text)
    {
        XSetForeground(d->display, d->gc, d->colorPen);

        XDrawString(d->display,
                    d->window,
                    d->gc,
                    x, y + textHeight(),
                    text.c_str(), text.length());
    }

    static const uint8_t xlogo16_bits[] = {
           0x0f, 0x80, 0x1e, 0x80, 0x3c, 0x40, 0x78, 0x20, 0x78, 0x10, 0xf0, 0x08,
           0xe0, 0x09, 0xc0, 0x05, 0xc0, 0x02, 0x40, 0x07, 0x20, 0x0f, 0x20, 0x1e,
           0x10, 0x1e, 0x08, 0x3c, 0x04, 0x78, 0x02, 0xf0};

    void WidgetLinux::drawImage(int32_t x, int32_t y, uint32_t width, uint32_t height, const std::string &path)
    {
        Pixmap bitmap = XCreateBitmapFromData(d->display, d->window,
                                              reinterpret_cast<const char*>(xlogo16_bits),
                                              16, 16);
//        Pixmap bitmap;
//        unsigned int bitmap_width, bitmap_height;
//        int hotspot_x, hotspot_y;
//        int rc = XReadBitmapFile(d->display, d->window,
//                                 path.c_str(),
//                                 &bitmap_width, &bitmap_height,
//                                 &bitmap,
//                                 &hotspot_x, &hotspot_y);
//        switch (rc) {
//        case BitmapOpenFailed: cout << "XReadBitmapFile - could not open file: " << path << endl; return;
//        case BitmapFileInvalid: cout << "XReadBitmapFile - file doesn't contain a valid bitmap: " << path << endl; return;
//        case BitmapNoMemory: cout << "XReadBitmapFile - not enough memory: " << path << endl; return;
//        }

        XCopyPlane(d->display, bitmap, d->window, d->gc,
                   0, 0,
                   16, 16,
                   x, y,
                   1);

        XSync(d->display, False);
        XFlush(d->display);
    }

    size_t WidgetLinux::textWidth(const std::string &text)
    {
        return textWidth(text.c_str(), text.size());
    }

    size_t WidgetLinux::textWidth(const char* text, size_t len)
    {
        return XTextWidth(d->font, text, len);
    }

    size_t WidgetLinux::textHeight()
    {
        return d->font->ascent + d->font->descent;
    }

    bool WidgetLinux::isHovered()
    {
        return d->isHovered;
    }

    void WidgetLinux::mainLoopEvent()
    {
        FD_ZERO(&d->inFileDescriptor);
        FD_SET(d->x11FileDescriptor, &d->inFileDescriptor);

        while(XPending(d->display))
        {
            XNextEvent(d->display, &d->event);

            if (d->event.xany.window == d->window)
            {
                procEvent(&d->event);
            }
            else
            {
                auto it = WIDGETS_MAP.find(d->event.xany.window);

                if (it != WIDGETS_MAP.end())
                    return it->second->procEvent(&d->event);
                else cout << "strange event: " << d->event.xany.window << endl;
            }
        }
    }

    void WidgetLinux::addEventListener(Object *object)
    {
        d->eventListners.push_back(object);
    }

    void WidgetLinux::procEvent(XEvent *event)
    {
        switch (event->type)
        {
        case KeyPress: keyEvent(&event->xkey, true); break;
        case KeyRelease: keyEvent(&event->xkey, false); break;
        case EnterNotify: sendEvent(SA::EventTypes::MouseHoverEvent, true); d->isHovered = true; break;
        case LeaveNotify: sendEvent(SA::EventTypes::MouseHoverEvent, false); d->isHovered = false; break;
        case ButtonPress:
        {
            switch (event->xbutton.button) {
            case Button1: mouseEvent(ButtonLeft, true); break;
            case Button2: mouseEvent(ButtonMiddle, true); break;
            case Button3: mouseEvent(ButtonRight, true); break;
            case Button4: mouseEvent(ButtonX1, true); break;
            case Button5: mouseEvent(ButtonX2, true); break;
            }
            break;
        }
        case ButtonRelease:
        {
            switch (event->xbutton.button) {
            case Button1: mouseEvent(ButtonLeft, false); break;
            case Button2: mouseEvent(ButtonMiddle, false); break;
            case Button3: mouseEvent(ButtonRight, false); break;
            case Button4: mouseEvent(ButtonX1, false); break;
            case Button5: mouseEvent(ButtonX2, false); break;
            }
            break;
        }
        case FocusIn: if (WIDGET_IN_FOCUS) WIDGET_IN_FOCUS->sendEvent(SA::EventTypes::FocusInEvent, true); break;
        case FocusOut: if (WIDGET_IN_FOCUS) WIDGET_IN_FOCUS->sendEvent(SA::EventTypes::FocusOutEvent, false); break;
        case MotionNotify: sendEvent(MouseMoveEvent, std::pair<int32_t, int32_t>(event->xmotion.x, event->xmotion.y)); break;
        case Expose: if (event->xexpose.count > 0) break; sendEvent(SA::EventTypes::PaintEvent, true); break;
        case ConfigureNotify: geometryUpdated(); break;
        case SelectionRequest: Clipboard::instance().onSelectionRequestEvent(event); break;
        case SelectionClear: break;
        case ClientMessage: SA::Application::instance().quit(); break;
        default: break;
        }
    }

    void WidgetLinux::sendEvent(EventTypes type, const std::any &value)
    {
        for (SA::Object *object: d->eventListners)
            object->event(type, value);
    }

    void WidgetLinux::focusEvent(bool state)
    {
        if (state)
        {
            if (WIDGET_IN_FOCUS && WIDGET_IN_FOCUS != this)
                WIDGET_IN_FOCUS->focusEvent(false);

            if (WIDGET_IN_FOCUS != this)
            {
                WIDGET_IN_FOCUS = this;
                WIDGET_IN_FOCUS->sendEvent(FocusInEvent, true);
            }
        }
        else if (WIDGET_IN_FOCUS)
        {
            WIDGET_IN_FOCUS->sendEvent(FocusOutEvent, false);
            WIDGET_IN_FOCUS = nullptr;
        }
    }

    void WidgetLinux::keyEvent(XKeyEvent *event, bool pressed)
    {
        KeySym key = XLookupKeysym(event, 0);
        Keys keycode = Key_Unknown;

        if (KEYS_MAP.find(key) != KEYS_MAP.end())
            keycode = KEYS_MAP.at(key);
        else cout << "keycode: " << std::hex << key << endl;

        KeyModifiers modifiers;
        modifiers.shift     = (event->state & ShiftMask);
        modifiers.alt       = (event->state & Mod1Mask);
        modifiers.ctrl      = (event->state & ControlMask);
        modifiers.super     = (event->state & Mod4Mask);
        modifiers.capsLock  = (event->state & LockMask);
        modifiers.numLock   = (event->state & Mod2Mask);

        if (WIDGET_IN_FOCUS)
            WIDGET_IN_FOCUS->sendEvent(EventTypes::KeyboardEvent, KeyEvent(keycode, modifiers, pressed));
    }

    void WidgetLinux::mouseEvent(MouseButton btn, bool pressed)
    {
        sendEvent(SA::EventTypes::MouseButtonEvent, MouseEvent(btn, pressed));
        if (pressed) focusEvent(true);
    }

    void WidgetLinux::geometryUpdated()
    {
        Window window;
        int32_t x, y;
        uint32_t width, height, border, depth;
        XGetGeometry(d->display, d->window, &window,
                     &x, &y, &width, &height,
                     &border, &depth);

        if (d->parent == nullptr)
        {
            Window child;
            XTranslateCoordinates( d->display, d->window, RootWindow(d->display, d->screen), 0, 0, &x, &y, &child);
        }

        if (x != d->x || y != d->y)
        {
            d->x = x;
            d->y = y;
            for (SA::Object *object: d->eventListners)
                object->event(SA::EventTypes::MoveEvent,
                              std::pair<int32_t, int32_t>(d->x, d->y));
        }

        if (width != d->width || height != d->height)
        {
            d->width = width;
            d->height = height;
            for (SA::Object *object: d->eventListners)
                object->event(SA::EventTypes::ResizeEvent,
                              std::pair<uint32_t, uint32_t>(d->width, d->height));
        }
    }

    void WidgetLinux::setWindowProperties()
    {
        //https://specifications.freedesktop.org/wm-spec/wm-spec-1.3.html
        Atom atoms[2] = { XInternAtom(d->display, "_NET_WM_WINDOW_TYPE_SPLASH", False), None };

        XChangeProperty(
                    d->display,
                    d->window,
                    XInternAtom(d->display, "_NET_WM_STATE", False),
                    XA_ATOM, 32, PropModeReplace, (unsigned char*)atoms, 1);

        // also
        // http://www.linuxhowtos.org/manpages/3/XSetTransientForHint.htm
    }
}

#endif //__linux__
