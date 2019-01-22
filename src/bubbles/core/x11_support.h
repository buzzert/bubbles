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

