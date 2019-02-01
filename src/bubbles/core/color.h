/*
 * Created on Thu Dec 20 2018
 *
 * Copyright (c) 2018 James Magahern <james@magahern.com>
 */

#pragma once

#include <bubbles/core/foundation.h>
#include <cairo/cairo.h>

BUBBLES_NAMESPACE_BEGIN

#define CAIRO_COLOR(color) \
    (color.red / 255.0), (color.green / 255.0), (color.blue / 255.0), (color.alpha / 255.0)

struct Color
{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    uint8_t alpha;

    Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
        : red(red), green(green), blue(blue), alpha(alpha) {};
    

    void set_source(cairo_t *cr) {
        cairo_set_source_rgba(cr, CAIRO_COLOR((*this)));
    }

};

namespace Colors {
    static Color black(0x00, 0x00, 0x00, 0xFF);
    static Color white(0xFF, 0xFF, 0xFF, 0xFF);
    static Color transparent(0x00, 0x00, 0x00, 0x00);
    static Color red(0xFF, 0x00, 0x00, 0xFF);
    static Color green(0x00, 0xFF, 0x00, 0xFF);
    static Color blue(0x00, 0x00, 0xFF, 0xFF);
}

BUBBLES_NAMESPACE_END
