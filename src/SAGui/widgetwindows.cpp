#ifdef WIN32

#include "SAGui/widgetwindows.h"
#include "SACore/application.h"
#include "SACore/global.h"

#include <windowsx.h>
#include <windows.h>
#include <winuser.h>
#include <iostream>
#include <tchar.h>
#include <vector>
#include <map>

using namespace std;

namespace SA
{
    static const std::map<unsigned int, SA::Keys> KEYS_MAP =
    {
        { VK_ESCAPE,    SA::Key_Escape      },
        { VK_TAB,       SA::Key_Tab         },
        { VK_BACK,      SA::Key_Backspace   },
        { VK_RETURN,    SA::Key_Return      },
        { VK_INSERT,    SA::Key_Insert      },
        { VK_DELETE,    SA::Key_Delete      },
        { VK_PAUSE,     SA::Key_Pause       },
        { VK_PRINT,     SA::Key_Print       },
        { VK_MODECHANGE,SA::Key_SysReq      },
        { VK_CLEAR,     SA::Key_Clear       },
        { VK_HOME,      SA::Key_Home        },
        { VK_END,       SA::Key_End         },
        { VK_LEFT,      SA::Key_Left        },
        { VK_UP,        SA::Key_Up          },
        { VK_RIGHT,     SA::Key_Right       },
        { VK_DOWN,      SA::Key_Down        },
        { VK_PRIOR,     SA::Key_PageUp      },
        { VK_NEXT,      SA::Key_PageDown    },
        { VK_SHIFT,     SA::Key_Shift       },
        { VK_CONTROL,   SA::Key_Control     },
        { VK_LCONTROL,  SA::Key_ControlL    },
        { VK_MENU,      SA::Key_Alt         },
        { VK_CAPITAL,   SA::Key_CapsLock    },
        { VK_NUMLOCK,   SA::Key_NumLock     },
        { VK_SCROLL,    SA::Key_ScrollLock  },
        { VK_F1,        SA::Key_F1          },
        { VK_F2,        SA::Key_F2          },
        { VK_F3,        SA::Key_F3          },
        { VK_F4,        SA::Key_F4          },
        { VK_F5,        SA::Key_F5          },
        { VK_F6,        SA::Key_F6          },
        { VK_F7,        SA::Key_F7          },
        { VK_F8,        SA::Key_F8          },
        { VK_F9,        SA::Key_F9          },
        { VK_F10,       SA::Key_F10         },
        { VK_F11,       SA::Key_F11         },
        { VK_F12,       SA::Key_F12         },
        { VK_F13,       SA::Key_F13         },
        { VK_F14,       SA::Key_F14         },
        { VK_F15,       SA::Key_F15         },
        { VK_F16,       SA::Key_F16         },
        { VK_F17,       SA::Key_F17         },
        { VK_F18,       SA::Key_F18         },
        { VK_F19,       SA::Key_F19         },
        { VK_F20,       SA::Key_F20         },
        { VK_F21,       SA::Key_F21         },
        { VK_F22,       SA::Key_F22         },
        { VK_F23,       SA::Key_F23         },
        { VK_F24,       SA::Key_F24         },
        { VK_LMENU,     SA::Key_LMenu       },
        { VK_RMENU,     SA::Key_RMenu       },
        { VK_HELP,      SA::Key_Help        },
        { VK_SPACE,     SA::Key_Space       },
        { VK_OEM_7,     SA::Key_Quote    },
        { VK_MULTIPLY,  SA::Key_Asterisk    },
        { VK_ADD,       SA::Key_Plus        },
        { VK_DECIMAL,   SA::Key_Comma       },
        { VK_SUBTRACT,  SA::Key_Minus       },
        { VK_SEPARATOR, SA::Key_Period      },
        { VK_DIVIDE,    SA::Key_Slash       },
        { 0x30,         SA::Key_0           },
        { 0x31,         SA::Key_1           },
        { 0x32,         SA::Key_2           },
        { 0x33,         SA::Key_3           },
        { 0x34,         SA::Key_4           },
        { 0x35,         SA::Key_5           },
        { 0x36,         SA::Key_6           },
        { 0x37,         SA::Key_7           },
        { 0x38,         SA::Key_8           },
        { 0x39,         SA::Key_9           },
        { VK_OEM_1,     SA::Key_Semicolon   },
        { 0x41,         SA::Key_A           },
        { 0x42,         SA::Key_B           },
        { 0x43,         SA::Key_C           },
        { 0x44,         SA::Key_D           },
        { 0x45,         SA::Key_E           },
        { 0x46,         SA::Key_F           },
        { 0x47,         SA::Key_G           },
        { 0x48,         SA::Key_H           },
        { 0x49,         SA::Key_I           },
        { 0x4A,         SA::Key_J           },
        { 0x4B,         SA::Key_K           },
        { 0x4C,         SA::Key_L           },
        { 0x4D,         SA::Key_M           },
        { 0x4E,         SA::Key_N           },
        { 0x4F,         SA::Key_O           },
        { 0x50,         SA::Key_P           },
        { 0x51,         SA::Key_Q           },
        { 0x52,         SA::Key_R           },
        { 0x53,         SA::Key_S           },
        { 0x54,         SA::Key_T           },
        { 0x55,         SA::Key_U           },
        { 0x56,         SA::Key_V           },
        { 0x57,         SA::Key_W           },
        { 0x58,         SA::Key_X           },
        { 0x59,         SA::Key_Y           },
        { 0x5A,         SA::Key_Z           },
        { VK_OEM_4,     SA::Key_BracketLeft },
        { VK_OEM_5,     SA::Key_Backslash   },
        { VK_OEM_6,     SA::Key_BracketRight},
        { VK_OEM_3,     SA::Key_AsciiTilde  },
        { VK_VOLUME_MUTE,       SA::Key_VolumeMute  },
        { VK_VOLUME_DOWN,       SA::Key_VolumeDown  },
        { VK_VOLUME_UP,         SA::Key_VolumeUp    },
        { VK_MEDIA_NEXT_TRACK,  SA::Key_MediaNext   },
        { VK_MEDIA_PREV_TRACK,  SA::Key_MediaPrev   },
        { VK_MEDIA_STOP,        SA::Key_MediaStop   },
        { VK_MEDIA_PLAY_PAUSE,  SA::Key_MediaPlay   }
    }; // KEYS_MAP

    static std::map<HWND, WidgetWindows*> WIDGETS_MAP;
    static WidgetWindows* WIDGET_IN_FOCUS = nullptr;

    LRESULT CALLBACK winproc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        auto it = WIDGETS_MAP.find(hwnd);

        if (it != WIDGETS_MAP.end())
            return it->second->windowProc(msg, wParam, lParam);
        else
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }

    struct WidgetWindows::WidgetWindowsPrivate
    {
        friend class SA::WidgetWindows;

        WidgetWindows *parent = nullptr;
        HWND hwnd = nullptr;
        HDC dc = nullptr;

        // paint event
        PAINTSTRUCT paintStruct;
        HDC paintingHandle = nullptr;

        HPEN pen = nullptr;
        HBRUSH brush = nullptr;
        HFONT font = nullptr;

        // geometry
        RECT rect;
        int x = 0;
        int y = 0;
        int width = 200;
        int height = 200;
        bool isHidden = false;
        bool isPosChanged = false;
        bool isHovered = false;

        std::vector<SA::Object*> eventListners;
    };

    WidgetWindows::WidgetWindows(WidgetWindows *parent):
        d(new WidgetWindowsPrivate)
    {
        d->parent = parent;

        HWND hwnd = NULL;
        DWORD style = WS_OVERLAPPEDWINDOW;
        const TCHAR CLSNAME_NAIN[] = TEXT("WidgetWindows");
        const TCHAR CLSNAME_CHILD[] = TEXT("Class");

        if (d->parent)
        {
            hwnd = d->parent->d->hwnd;
            style = WS_CHILD | WS_VISIBLE;
        }

        WNDCLASSEX wc = { };
        HINSTANCE hInst = GetModuleHandle(NULL);

        wc.cbSize        = sizeof (wc);
        wc.style         = 0;
        wc.lpfnWndProc   = winproc;
        wc.cbClsExtra    = 0;
        wc.cbWndExtra    = 0;
        wc.hInstance     = hInst;
        wc.hIcon         = LoadIcon (NULL, IDI_APPLICATION);
        wc.hCursor       = LoadCursor (NULL, IDC_ARROW);
        wc.hbrBackground = (HBRUSH) GetStockObject (LTGRAY_BRUSH);
        wc.lpszMenuName  = NULL;
        wc.lpszClassName = d->parent ? CLSNAME_NAIN : CLSNAME_CHILD;
        wc.hIconSm       = LoadIcon (NULL, IDI_APPLICATION);

        if (!GetClassInfoEx(hInst, wc.lpszClassName, &wc))
        {
            if (!RegisterClassEx(&wc))
            {
                DWORD errCode = GetLastError();

                MessageBox(NULL, TEXT("Could not register window class"),
                           NULL, MB_ICONERROR);

                cout << __PRETTY_FUNCTION__ << errCode << endl;
                return;
            }
        }

        d->hwnd = CreateWindowEx(WS_EX_LEFT,
                                 d->parent ? CLSNAME_NAIN : CLSNAME_CHILD,
                                 NULL,
                                 style,
                                 CW_USEDEFAULT,
                                 CW_USEDEFAULT,
                                 CW_USEDEFAULT,
                                 CW_USEDEFAULT,
                                 hwnd,
                                 NULL,
                                 hInst,
                                 NULL);
        if (!d->hwnd)
        {
            MessageBox(NULL, TEXT("Could not create window"), NULL, MB_ICONERROR);
            SA::Application::instance().quit();
            return;
        }

        WIDGETS_MAP.insert({d->hwnd, this});

        RECT rect;
        if (GetWindowRect(d->hwnd, &rect))
        {
            d->x = rect.left;
            d->y = rect.top;
            d->width = rect.right - rect.left;
            d->height = rect.bottom - rect.top;
        }

        d->dc = GetDC(d->hwnd);
        d->paintStruct.hdc = d->dc;

        setFont();
        update();
    }

    WidgetWindows::~WidgetWindows()
    {
        DeleteDC(d->dc);
        WIDGETS_MAP.erase(WIDGETS_MAP.find(d->hwnd));
    }

    void WidgetWindows::show()
    {
        d->isHidden = false;

        ShowWindow(d->hwnd, SW_SHOWNORMAL);
        UpdateWindow(d->hwnd);
        update();
    }

    void WidgetWindows::hide()
    {
        d->isHidden = true;

        ShowWindow(d->hwnd, SW_HIDE);
        UpdateWindow(d->hwnd);
    }

    void WidgetWindows::update()
    {
        InvalidateRect(d->hwnd, NULL, TRUE);
    }

    void WidgetWindows::setTitle(const std::string &title)
    {
        SetWindowText(d->hwnd, title.c_str());
    }

    void WidgetWindows::move(int x, int y)
    {
        d->x = x;
        d->y = y;
        MoveWindow(d->hwnd, d->x, d->y, d->width, d->height, true);
        update();
    }

    void WidgetWindows::resize(int w, int h)
    {
        d->width = w;
        d->height = h;
        MoveWindow(d->hwnd, d->x, d->y, d->width, d->height, true);
        update();
    }

    void WidgetWindows::setGeometry(int x, int y, int w, int h)
    {
        d->x = x;
        d->y = y;
        d->width = w;
        d->height = h;
        MoveWindow(d->hwnd, d->x, d->y, d->width, d->height, true);
        update();
    }

    int WidgetWindows::x()
    {
        return d->x;
    }

    int WidgetWindows::y()
    {
        return d->y;
    }

    int WidgetWindows::width()
    {
        return d->width;
    }

    int WidgetWindows::height()
    {
        return d->height;
    }

    void WidgetWindows::setPen(unsigned char red, unsigned char green,
                               unsigned char blue, unsigned int width)
    {
        if (!d->paintingHandle) return;

        SetBkMode(d->paintingHandle, TRANSPARENT);
        SetTextColor(d->paintingHandle, RGB(red, green, blue));

        if (d->pen) DeleteObject(d->pen);
        d->pen = CreatePen(PS_SOLID, width, RGB(red, green, blue));
    }

    void WidgetWindows::setBrush(unsigned char red, unsigned char green, unsigned char blue)
    {
        if (!d->paintingHandle) return;

        if (d->brush) DeleteObject(d->brush);
        d->brush = CreateSolidBrush(RGB(red,green,blue));
    }

    void WidgetWindows::setFont()
    {
        // https://docs.microsoft.com/ru-ru/windows/win32/gdi/using-a-stock-font-to-draw-text

        if (d->font) DeleteObject(d->font);
        d->font = (HFONT)GetStockObject(ANSI_VAR_FONT);
    }

    void WidgetWindows::drawLine(int x1, int y1, int x2, int y2)
    {
        if (!d->paintingHandle) return;

        SelectObject(d->paintingHandle, d->pen);
        MoveToEx(d->paintingHandle, x1, y1, (LPPOINT) NULL);
        LineTo(d->paintingHandle, x2, y2);
    }

    void WidgetWindows::drawRect(int x, int y, int width, int height)
    {
        if (!d->paintingHandle) return;

        SelectObject(d->paintingHandle, d->pen);
        SelectObject(d->paintingHandle, d->brush);

        Rectangle(d->paintingHandle, x, y, x + width, y + height);
    }

    void WidgetWindows::drawText(int x, int y, const std::string &text)
    {
        if (!d->paintingHandle) return;

        HFONT fontTmp = (HFONT)SelectObject(d->dc, d->font);
        if (fontTmp)
        {
            TextOut(d->paintingHandle, x, y, text.c_str(), text.size());
            SelectObject(d->dc, fontTmp);
        }
    }

    size_t WidgetWindows::textWidth(const std::string &text)
    {
        return textWidth(text.c_str(), text.size());
    }

    size_t WidgetWindows::textWidth(const char *text, size_t len)
    {
        SIZE textSize;
        GetTextExtentPoint32(d->dc, text, len, &textSize);
        return static_cast<int>(textSize.cx);
    }

    size_t WidgetWindows::textHeight()
    {
        SIZE textSize;
        GetTextExtentPoint32(d->dc, " ", 1, &textSize);
        return static_cast<size_t>(textSize.cy);
    }

    bool WidgetWindows::isHovered()
    {
        return d->isHovered;
    }

    void WidgetWindows::mainLoopEvent()
    {
        MSG msg;
        while (PeekMessage (&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage (&msg);
            DispatchMessage (&msg);
        }
    }

    void WidgetWindows::addEventListener(Object *object)
    {
        d->eventListners.push_back(object);
    }

    int WidgetWindows::windowProc(unsigned int msg, unsigned int &wParam, long &lParam)
    {
        switch(msg)
        {
        case WM_DESTROY: SA::Application::instance().quit(); break;
        case WM_QUIT:    SA::Application::instance().quit(); break;
        case WM_SIZE: geometryUpdated(); break;
        case WM_MOVE: geometryUpdated(); break;
        case WM_KEYDOWN: keyEvent(wParam, true); break;
        case WM_KEYUP: keyEvent(wParam, false); break;
        case WM_LBUTTONDOWN: mouseEvent(ButtonLeft, true); break;
        case WM_RBUTTONDOWN: mouseEvent(ButtonRight, true); break;
        case WM_MBUTTONDOWN: mouseEvent(ButtonMiddle, true); break;
        case WM_XBUTTONDOWN: mouseEvent(ButtonX1, true); break;
        case WM_LBUTTONUP: mouseEvent(ButtonLeft, false); break;
        case WM_RBUTTONUP: mouseEvent(ButtonRight, false); break;
        case WM_MBUTTONUP: mouseEvent(ButtonMiddle, false); break;
        case WM_XBUTTONUP: mouseEvent(ButtonX1, false); break;
        case WM_MOUSEMOVE:
        {
            sendEvent(MouseMoveEvent,
                      std::pair<int, int>(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)));

            if (!d->isHovered)
            {
                TRACKMOUSEEVENT tme = {0};
                tme.cbSize = sizeof(TRACKMOUSEEVENT);
                tme.dwFlags = TME_HOVER | TME_LEAVE;
                tme.hwndTrack = d->hwnd;
                tme.dwHoverTime = HOVER_DEFAULT;
                TrackMouseEvent(&tme);
                d->isHovered = true;
                sendEvent(MouseHoverEvent, true);
            }
            break;
        }
        case WM_MOUSELEAVE:
        {
            d->isHovered = false;
            sendEvent(MouseHoverEvent, false);
            break;
        }
        case WM_SETFOCUS: if (WIDGET_IN_FOCUS) WIDGET_IN_FOCUS->sendEvent(FocusInEvent, true); break;
        case WM_KILLFOCUS: if (WIDGET_IN_FOCUS) WIDGET_IN_FOCUS->sendEvent(FocusOutEvent, false); break;
        case WM_PAINT:
        {
            HDC tmpDC = BeginPaint(d->hwnd, &d->paintStruct);

            d->paintingHandle = CreateCompatibleDC(tmpDC);
            HBITMAP memBM = CreateCompatibleBitmap(tmpDC, d->width, d->height);
            SelectObject(d->paintingHandle, memBM);
            FillRect(d->paintingHandle, &d->rect, (HBRUSH)GetStockObject(DC_BRUSH));

            sendEvent(PaintEvent, true);

            BitBlt(tmpDC, d->x, d->y, d->width, d->height, d->paintingHandle, 0, 0, SRCCOPY);

            EndPaint(d->hwnd, &d->paintStruct);
            DeleteDC(d->paintingHandle);
            DeleteObject(memBM);
            break;
        }
        case WM_ERASEBKGND: return 1;
        }

        //        cout << __PRETTY_FUNCTION__ << " " << this << endl;

        return DefWindowProc(d->hwnd, msg, wParam, lParam);
    }

    void WidgetWindows::sendEvent(EventTypes type, const any &value)
    {
        for (Object *object: d->eventListners)
            object->event(type, value);
    }

    void WidgetWindows::focusEvent(bool state)
    {
        if (state)
        {
            if (WIDGET_IN_FOCUS && WIDGET_IN_FOCUS != this)
                WIDGET_IN_FOCUS->focusEvent(false);

            WIDGET_IN_FOCUS = this;
            WIDGET_IN_FOCUS->sendEvent(FocusInEvent, true);
        }
        else if (WIDGET_IN_FOCUS)
        {
            WIDGET_IN_FOCUS->sendEvent(FocusOutEvent, false);
            WIDGET_IN_FOCUS = nullptr;
        }
    }

    void WidgetWindows::keyEvent(unsigned int param, bool pressed)
    {
        Keys keycode = Key_Unknown;

        if (KEYS_MAP.find(param) != KEYS_MAP.end())
            keycode = KEYS_MAP.at(param);

        KeyModifiers modifiers;
        modifiers.shift     = (GetKeyState(VK_SHIFT) & 0x8000);
        modifiers.ctrl      = (GetKeyState(VK_CONTROL) & 0x8000);
        modifiers.alt       = (GetKeyState(VK_MENU) & 0x8000);
        modifiers.capsLock  = (GetKeyState(VK_CAPITAL) & 0x0001);
        modifiers.numLock   = (GetKeyState(VK_NUMLOCK) & 0x0001);

        if (WIDGET_IN_FOCUS)
            WIDGET_IN_FOCUS->sendEvent(EventTypes::KeyboardEvent, KeyEvent(keycode, modifiers, pressed));
    }

    void WidgetWindows::mouseEvent(MouseButton btn, bool pressed)
    {
        sendEvent(SA::EventTypes::MouseButtonEvent, MouseEvent(btn, pressed));
        if (pressed) focusEvent(true);
    }

    void WidgetWindows::geometryUpdated()
    {
        GetClientRect(d->hwnd, &d->rect);
        d->paintStruct.rcPaint = d->rect;

        int x = d->rect.left;
        int y = d->rect.top;
        int width = d->rect.right - d->rect.left;
        int height = d->rect.bottom - d->rect.top;

        if (x != d->x || y != d->y)
        {
            d->x = x;
            d->y = y;
            sendEvent(MoveEvent,
                      std::pair<int, int>(d->x, d->y));
        }

        if (width != d->width || height != d->height)
        {
            d->width = static_cast<int>(width);
            d->height = static_cast<int>(height);
            sendEvent(ResizeEvent,
                      std::pair<int, int>(d->width, d->height));
        }
    }
}

#endif //WIN32
