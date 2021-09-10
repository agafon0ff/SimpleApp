#include <iostream>
#include "clipboard.h"

namespace SA
{
    static const Atom XA_ATOM = 4, XA_STRING = 31;

    struct Clipboard::ClipboardPrivate
    {
        Display *display = nullptr;
        Window window;
        std::string text;
        Atom selection = 0L;
    };

    Clipboard &Clipboard::instance()
    {
        static Clipboard * const ptr = new Clipboard();
        return *ptr;
    }

    std::string Clipboard::getText()
    {
        if (!d->display) return std::string();

        XEvent event;
        int format;
        unsigned long N, size;
        char *data;
        Atom target;
        Atom CLIPBOARD = XInternAtom(d->display, "CLIPBOARD", 0);
        Atom XSEL_DATA = XInternAtom(d->display, "XSEL_DATA", 0);
        Atom FMT_NAME  = XInternAtom(d->display, "UTF8_STRING", 1);

        if(FMT_NAME == None) FMT_NAME = XA_STRING;

        XConvertSelection(d->display, CLIPBOARD, FMT_NAME, XSEL_DATA, d->window, CurrentTime);

        XSync(d->display, 0);
        XNextEvent(d->display, &event);

        switch(event.type)
        {
            case SelectionNotify:
            if(event.xselection.selection != CLIPBOARD) break;
            if(event.xselection.property)
            {
                XGetWindowProperty(event.xselection.display,
                                   event.xselection.requestor,
                                   event.xselection.property,
                                   0L,(~0L), 0, AnyPropertyType,
                                   &target, &format, &size, &N,
                                   (unsigned char**)&data);

                if(target == FMT_NAME)
                {
                    d->text = std::string(data, size);
                    XFree(data);
                }

                XDeleteProperty(event.xselection.display, event.xselection.requestor, event.xselection.property);
            }
        case SelectionRequest: return d->text;  // copying own data
       }

        return d->text;
    }

    void Clipboard::setText(const std::string &text)
    {
        if (!d->display) return;
        d->text = text;

        d->selection = XInternAtom(d->display, "CLIPBOARD", 0);
        XSetSelectionOwner (d->display, d->selection, d->window, 0);
        if (XGetSelectionOwner (d->display, d->selection) != d->window)
        {
            d->text.clear();
            return;
        }
    }

    void Clipboard::setNativePointers(Display *display_, Window window_)
    {
        if (d->display) return;

        d->display = display_;
        d->window = window_;
    }

    void Clipboard::onSelectionRequestEvent(XEvent *event)
    {
        if (!d->display) return;
        if (d->text.empty()) return;

        if (event->xselectionrequest.selection != d->selection)return;

        Atom targets_atom = XInternAtom(d->display, "TARGETS", 0);
        Atom text_atom = XInternAtom(d->display, "TEXT", 0);
        Atom UTF8 = XInternAtom(d->display, "UTF8_STRING", 1);
        if (UTF8 == None) UTF8 = XA_STRING;

        XSelectionRequestEvent * xsr = &event->xselectionrequest;
        int R = 0;

        XSelectionEvent ev = {0};
        ev.type = SelectionNotify;
        ev.display = xsr->display;
        ev.requestor = xsr->requestor;
        ev.selection = xsr->selection;
        ev.time = xsr->time;
        ev.target = xsr->target;
        ev.property = xsr->property;

        if (ev.target == targets_atom)
        {
            R = XChangeProperty (ev.display, ev.requestor, ev.property,
                                 XA_ATOM, 32, PropModeReplace,
                                 (unsigned char*)&UTF8, 1);
        }
        else if (ev.target == XA_STRING || ev.target == text_atom)
        {
            R = XChangeProperty(ev.display, ev.requestor, ev.property,
                                XA_STRING, 8, PropModeReplace,
                                reinterpret_cast<const unsigned char *>(d->text.data()),
                                d->text.size());
        }
        else if (ev.target == UTF8)
        {
            R = XChangeProperty(ev.display, ev.requestor, ev.property,
                                UTF8, 8, PropModeReplace,
                                reinterpret_cast<const unsigned char *>(d->text.data()),
                                d->text.size());
        }
        else ev.property = None;

        if ((R & 2) == 0)
            XSendEvent (d->display, ev.requestor, 0, 0, (XEvent *)&ev);
    }

    Clipboard::Clipboard() : d(new ClipboardPrivate)
    {
    }

    Clipboard::~Clipboard()
    {
        delete d;
    }

} // namespace SA
