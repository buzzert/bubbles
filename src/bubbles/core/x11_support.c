/*
 * x11_support.c
 *
 * Created by buzzert <buzzert@buzzert.net> 2019-01-18
 */

#include "x11_support.h"

#include <X11/Xatom.h>
#include <X11/extensions/Xfixes.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static Window __window = { 0 };
static Display *__display = NULL;

static void *__pointer_callback_context = NULL;
static x11_pointer_callback_t __pointer_callback = NULL;

static Window get_window_from_environment_or_make_one(Display *display, int width, int height)
{
    Window window;

    const char *env_window = getenv("XSCREENSAVER_WINDOW");
    if (env_window != NULL && env_window[0] != 0) {
        char *endptr = NULL;
        unsigned long long number = strtoull(env_window, &endptr, 0);
        window = (Window)number;
    } else {
        // Presumably this is for debugging
        Window root_window = DefaultRootWindow(__display);
        window = XCreateSimpleWindow(
                display,        // display
                root_window,    // parent window
                0, 0,           // x, y
                width,          // width
                height,         // height
                0,              // border_width
                0,              // border
                0               // background
        );
    }

    return window;
}

static void x11_set_window_type()
{
    Atom prop_atom = XInternAtom(__display, "_NET_WM_WINDOW_TYPE", false);
    Atom type_atom = XInternAtom(__display, "_NET_WM_WINDOW_TYPE_DIALOG", false);
    XChangeProperty(
        __display,
        __window,
        prop_atom,
        XA_ATOM,
        32,
        PropModeReplace,
        (unsigned char *)&type_atom,
        1
    );
}

void x11_get_display_bounds(int *width, int *height)
{
    *width = DisplayWidth(__display, DefaultScreen(__display));
    *height = DisplayHeight(__display, DefaultScreen(__display));
}

void x11_helper_set_fullscreen(bool fullscreen)
{
    // Talk to the window manager to ask to go fullscreen
    Atom wm_state = XInternAtom(__display, "_NET_WM_STATE", false);
    Atom atom_fullscreen = XInternAtom(__display, "_NET_WM_STATE_FULLSCREEN", false);

    XEvent xev;
    memset(&xev, 0, sizeof(xev));
    xev.type = ClientMessage;
    xev.xclient.window = __window;
    xev.xclient.message_type = wm_state;
    xev.xclient.format = 32;
    xev.xclient.data.l[0] = 1;
    xev.xclient.data.l[1] = atom_fullscreen;
    xev.xclient.data.l[2] = 0;

    XSendEvent(__display, DefaultRootWindow(__display), False,
                    SubstructureRedirectMask | SubstructureNotifyMask, &xev);
}

cairo_surface_t* x11_helper_acquire_cairo_surface(int width, int height)
{
    __display = XOpenDisplay(NULL);
    if (__display == NULL) {
        fprintf(stderr, "Error opening display\n");
        return NULL;
    }

    // Create (or get) window
    __window = get_window_from_environment_or_make_one(__display, width, height);

    // Set window type hint
    x11_set_window_type();

    // Enable key/mouse button events
    XSelectInput(__display, __window,
        (ButtonPressMask | ButtonReleaseMask | KeyPressMask | StructureNotifyMask)
    );

    // Map window to display
    XMapWindow(__display, __window);

    // Create cairo surface
    int screen = DefaultScreen(__display);
    Visual *visual = DefaultVisual(__display, screen);
    cairo_surface_t *surface = cairo_xlib_surface_create(
            __display,
            __window,
            visual,
            width,
            height
    );

    return surface;
}

void x11_helper_destroy_surface(cairo_surface_t *surface)
{
    cairo_surface_destroy(surface);
    XCloseDisplay(__display);
}

void x11_set_cursor_visible(bool cursor_visible)
{
    if (cursor_visible) {
        XFixesShowCursor(__display, __window);
    } else {
        XFixesHideCursor(__display, __window);
    }
}

/*
 * Event handling
 */

static void x11_handle_button_event(XButtonEvent *event, bool pressed)
{
    if (__pointer_callback != NULL) {
        __pointer_callback(__pointer_callback_context, event->x, event->y, pressed);
    }
}

void x11_register_pointer_callback(x11_pointer_callback_t callback, void *context)
{
    __pointer_callback = callback;
    __pointer_callback_context = context;
}

void x11_poll_events()
{
    XEvent event;

    if (XPending(__display)) {
        XNextEvent(__display, &event);

        switch (event.type) {
            case ButtonPress:
                x11_handle_button_event(&event, true);
                break;
            case ButtonRelease:
                x11_handle_button_event(&event, false);
                break;
            default:
                break;
        }
    }
}

