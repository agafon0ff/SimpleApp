#include "SACore/application.h"
#include "SAGui/widgetlinux.h"

#ifdef __linux__
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/Xatom.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <list>
#include <map>

extern int errno;

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
    { XK_A,             SA::Key_A           },
    { XK_B,             SA::Key_B           },
    { XK_C,             SA::Key_C           },
    { XK_D,             SA::Key_D           },
    { XK_E,             SA::Key_E           },
    { XK_F,             SA::Key_F           },
    { XK_G,             SA::Key_G           },
    { XK_H,             SA::Key_H           },
    { XK_I,             SA::Key_I           },
    { XK_J,             SA::Key_J           },
    { XK_K,             SA::Key_K           },
    { XK_L,             SA::Key_L           },
    { XK_M,             SA::Key_M           },
    { XK_N,             SA::Key_N           },
    { XK_O,             SA::Key_O           },
    { XK_P,             SA::Key_P           },
    { XK_Q,             SA::Key_Q           },
    { XK_R,             SA::Key_R           },
    { XK_S,             SA::Key_S           },
    { XK_T,             SA::Key_T           },
    { XK_U,             SA::Key_U           },
    { XK_V,             SA::Key_V           },
    { XK_W,             SA::Key_W           },
    { XK_X,             SA::Key_X           },
    { XK_Y,             SA::Key_Y           },
    { XK_Z,             SA::Key_Z           },
    { XK_braceleft,     SA::Key_BracketLeft },
    { XK_backslash,     SA::Key_Backslash   },
    { XK_braceright,    SA::Key_BracketRight},
    { XK_asciitilde,    SA::Key_AsciiTilde  }
//    { XK_Mute,       SA::Key_VolumeMute  },
//    { XK_VOLUME_DOWN,       SA::Key_VolumeDown  },
//    { XK_VOLUME_UP,         SA::Key_VolumeUp    },
//    { XK_MEDIA_NEXT_TRACK,  SA::Key_MediaNext   },
//    { XK_MEDIA_PREV_TRACK,  SA::Key_MediaPrev   },
//    { XK_MEDIA_STOP,        SA::Key_MediaStop   },
//    { XK_MEDIA_PLAY_PAUSE,  SA::Key_MediaPlay   }
};

namespace SA
{
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

        int x = 0;
        int y = 0;
        int width = 200;
        int height = 200;
        bool isHidden = false;
        bool isPosChanged = false;

        unsigned long widthPen = 1L;
        unsigned long colorPen = 0L;
        unsigned long colorBrush = 0L;

        std::list<SA::Object*> eventListners;
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
                                            d->x, d->y, d->width, d->height, 1,
                                            XBlackPixel(d->display, d->screen),
                                            XWhitePixel(d->display, d->screen));



            show();
        }

        /* What events need to be responded to */
        XSelectInput(d->display, d->window, ExposureMask |
                     KeyPressMask |
                     KeyReleaseMask |
                     ButtonPressMask |
                     ButtonReleaseMask |
                     PointerMotionMask |
                     StructureNotifyMask |
                     SubstructureNotifyMask |
                     VisibilityChangeMask);

        d->gc = DefaultGC(d->display, d->screen);
        d->x11FileDescriptor = ConnectionNumber(d->display);

        setFont();
    }

    WidgetLinux::~WidgetLinux()
    {
        if (d->font)
            XFreeFont(d->display, d->font);

        XDestroyWindow(d->display, d->window);
        if (d->parent) return;
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
        XSendEvent(d->display, d->window, False, ExposureMask, &event);
    }

    void WidgetLinux::setTitle(const std::string &title)
    {
        XStoreName(d->display, d->window, title.c_str());
    }

    void WidgetLinux::move(int x, int y)
    {
        d->x = x;
        d->y = y;
        d->isPosChanged = true;
        XMoveWindow(d->display, d->window, d->x, d->y);
    }

    void WidgetLinux::resize(int width, int height)
    {
        d->width = width;
        d->height = height;
        XResizeWindow(d->display, d->window, d->width, d->height);
    }

    void WidgetLinux::setGeometry(int x, int y, int w, int h)
    {
        d->x = x;
        d->y = y;
        d->width = w;
        d->height = h;
        d->isPosChanged = true;
        XMoveResizeWindow(d->display, d->window, d->x, d->y, d->width, d->height);
    }

    int WidgetLinux::x()
    {
        return d->x;
    }

    int WidgetLinux::y()
    {
        return d->y;
    }

    int WidgetLinux::width()
    {
        return d->width;
    }

    int WidgetLinux::height()
    {
        return d->height;
    }

    void WidgetLinux::setPen(unsigned int width, unsigned char red,
                                unsigned char green, unsigned char blue)
    {
        d->widthPen = width;
        d->colorPen = 0L;
        d->colorPen += red;
        d->colorPen <<= 8;
        d->colorPen += green;
        d->colorPen <<= 8;
        d->colorPen += blue;
    }

    void WidgetLinux::setBrush(unsigned char red, unsigned char green, unsigned char blue)
    {
        d->colorBrush = 0L;
        d->colorBrush += red;
        d->colorBrush <<= 8;
        d->colorBrush += green;
        d->colorBrush <<= 8;
        d->colorBrush += blue;
    }

    void WidgetLinux::setFont()
    {
        // https://www.itec.suny.edu/scsys/vms/ovmsdoc073/v73/5642/5642pro_011.html

        if (d->font)
        {
            XFreeFont(d->display, d->font);
            d->font = nullptr;
        }


        const char *fontname = "-*-*-*-*-*--20-*-*-*-*-*-*-*";
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

    void WidgetLinux::drawLine(int x1, int y1, int x2, int y2)
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

    void WidgetLinux::drawRect(int x, int y, int width, int height)
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
    }

    void WidgetLinux::drawText(int x, int y, const std::string &text)
    {
        XSetForeground(d->display, d->gc, d->colorPen);

        XDrawString(d->display,
                    d->window,
                    d->gc,
                    x, y, text.c_str(),
                    text.length());
    }

    void WidgetLinux::mainLoopEvent()
    {
        FD_ZERO(&d->inFileDescriptor);
        FD_SET(d->x11FileDescriptor, &d->inFileDescriptor);

        while(XPending(d->display))
        {
            XNextEvent(d->display, &d->event);

            switch (d->event.type)
            {
            case KeyPress:
            {
                if (KEYS_MAP.find(d->event.xbutton.button) == KEYS_MAP.end())
                {
                    sendEvent(SA::EventTypes::ButtonPressEvent,
                              static_cast<unsigned int>(SA::Key_Unknown + d->event.xbutton.button));
                }
                else
                {
                    sendEvent(SA::EventTypes::ButtonPressEvent,
                               static_cast<unsigned int>(KEYS_MAP.at(d->event.xbutton.button)));
                }

                break;
            }
            case KeyRelease:
            {
                std::cout << __PRETTY_FUNCTION__ << " keyCode: " << std::hex << d->event.xbutton.button << std::endl;

                if (KEYS_MAP.find(d->event.xbutton.button) == KEYS_MAP.end())
                {
                    sendEvent(SA::EventTypes::ButtonReleaseEvent,
                              static_cast<unsigned int>(SA::Key_Unknown + d->event.xbutton.button));
                }
                else
                {
                    sendEvent(SA::EventTypes::ButtonReleaseEvent,
                               static_cast<unsigned int>(KEYS_MAP.at(d->event.xbutton.button)));
                }

                break;
            }
            case Expose:
            {
                for (SA::Object *object: d->eventListners)
                    object->event(SA::EventTypes::PaintEvent, true);
                break;
            }
            case ConfigureNotify:
            {
                geometryUpdated();
                break;
            }
            case ClientMessage:
            {
                SA::Application::instance().quit();
                break;
            }
            default:
                break;
            }
        }
    }

    void WidgetLinux::addEventListener(Object *object)
    {
        d->eventListners.push_back(object);
    }

    void WidgetLinux::sendEvent(EventTypes type, const std::any &value)
    {
        for (SA::Object *object: d->eventListners)
            object->event(type, value);
    }

    void WidgetLinux::geometryUpdated()
    {
        Window window;
        int x, y;
        unsigned int width, height, border, depth;
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
                              std::pair<int, int>(d->x, d->y));
        }

        if (width != d->width || height != d->height)
        {
            d->width = static_cast<int>(width);
            d->height = static_cast<int>(height);
            for (SA::Object *object: d->eventListners)
                object->event(SA::EventTypes::ResizeEvent,
                              std::pair<int, int>(d->width, d->height));
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
