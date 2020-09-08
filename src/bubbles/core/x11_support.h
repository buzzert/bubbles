/*
 * x11_support.h
 *
 * Relevant helper functions for acquiring a drawing surface on X11
 * Created by buzzert <buzzert@buzzert.net> 2019-01-18
 */

#pragma once

#include <cairo/cairo.h>
#include <cairo-xlib.h>
#include <stdbool.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

extern void x11_get_display_bounds(int *width, int *height);

// Sets up a window and returns a cairo_surface to draw onto
extern cairo_surface_t* x11_helper_acquire_cairo_surface(int width, int height);

// Call this to go fullscreen
extern void x11_helper_set_fullscreen(bool fullscreen);

// Cleanup
extern void x11_helper_destroy_surface(cairo_surface_t *surface);

// Show/hide the mouse cursor
extern void x11_set_cursor_visible(bool cursor_visible);

// Poll for events (call from main runloop)
extern void x11_poll_events();

// Callback function types for events
typedef void(*x11_window_delete_callback_t)(void *context);
typedef void(*x11_window_expose_callback_t)(void *context);
typedef void(*x11_pointer_callback_t)(void *context, int x, int y, bool pressed);

typedef struct {
    void *context;

    x11_pointer_callback_t pointer_callback;
    x11_window_delete_callback_t window_delete_callback;
    x11_window_expose_callback_t window_expose_callback;
} x11_callbacks_t;

extern void x11_register_callbacks(x11_callbacks_t callbacks);

