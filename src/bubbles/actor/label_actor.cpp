/*
 * Created on Thu Dec 20 2018
 *
 * Copyright (c) 2018 James Magahern <james@magahern.com>
 */

#include "label_actor.h"
#include <bubbles/scene/scene.h>

BUBBLES_NAMESPACE_BEGIN

#define DEFAULT_FONT_PROP "Karla 24"

LabelActor::LabelActor(Rect rect, std::string contents)
    : Actor(rect), _contents(contents),
      _font_prop(DEFAULT_FONT_PROP),
      _alignment(PANGO_ALIGN_LEFT),
      _padding(0, 0)
{
}

LabelActor::~LabelActor()
{
    g_object_unref(_pango_layout);
}

void LabelActor::set_contents(std::string contents)
{
    if (_contents != contents) {
        _contents = contents;
        _pango_layout_valid = false;
        set_needs_display();
    }
}

const std::string& LabelActor::get_contents() const
{
    return _contents;
}

void LabelActor::set_foreground_color(Color c)
{
    _foreground_color = c;
    set_needs_display();
}

const Color& LabelActor::get_foreground_color() const
{
    return _foreground_color;
}

void LabelActor::set_font_prop(std::string font_prop)
{
    if (_font_prop != font_prop) {
        _font_prop = font_prop;
        _pango_layout_valid = false;
        set_needs_display();
    }
}

const std::string& LabelActor::get_font_prop() const
{
    return _font_prop;
}

void LabelActor::set_alignment(PangoAlignment alignment)
{
    if (_alignment != alignment) {
        _alignment = alignment;
        _pango_layout_valid = false;
        set_needs_display();
    }
}

const PangoAlignment& LabelActor::get_alignment() const
{
    return _alignment;
}

const Size& LabelActor::get_padding() const
{
    return _padding;
}

void LabelActor::set_padding(const Size padding)
{
    _padding = padding;
}

PangoLayout* LabelActor::get_pango_layout(cairo_t *cr)
{
    if (_pango_layout == nullptr || !_pango_layout_valid) {
        PangoLayout *pango_layout = pango_cairo_create_layout(cr);

        PangoFontDescription *pango_desc = pango_font_description_from_string(_font_prop.c_str());
        pango_layout_set_font_description(pango_layout, pango_desc);
        pango_font_description_free(pango_desc);

        pango_layout_set_markup(pango_layout, _contents.c_str(), -1);

        // This actually only really affects how multiple lines work
        // If we're trying to draw text centered, we actually need to do calculation
        pango_layout_set_alignment(pango_layout, _alignment);

        if (_pango_layout != nullptr) {
            g_object_unref(_pango_layout);
        }
        
        _pango_layout = pango_layout;
        _pango_layout_valid = true;
    }

    return _pango_layout;
}

Size LabelActor::get_intrinsic_size()
{
    int layout_width = 0;
    int layout_height = 0;

    if (_pango_layout == nullptr) {
        cairo_t *cr = get_inherited_cairo_context();
        get_pango_layout(cr);
    }

    if (_pango_layout != nullptr) {
        pango_layout_get_pixel_size(_pango_layout, &layout_width, &layout_height);
    }

    return Size(layout_width + (_padding.width * 2), layout_height + (_padding.height * 2));
}

void LabelActor::render(cairo_t *cr, Rect at_rect)
{
    Actor::render(cr, at_rect);

    PangoLayout *pango_layout = get_pango_layout(cr);
    Size intrinsic_size = get_intrinsic_size();

    double offset_x = _padding.width;
    double offset_y = _padding.height;
    const double padding_adjust = _padding.width / 2.0;
    if (_alignment == PANGO_ALIGN_CENTER) {
        offset_x = (rect.width - (intrinsic_size.width - _padding.width * 2)) / 2.0;
    } else if (_alignment == PANGO_ALIGN_RIGHT) {
        offset_x = (rect.width - intrinsic_size.width) + padding_adjust;
    }

    // Maybe make this an option, but for now always center vertically
    offset_y = (rect.height - intrinsic_size.height) / 2.0;

    Color &color = _foreground_color;
    color.alpha = static_cast<uint8_t>(_alpha * 0xFF);
    cairo_set_source_rgba(cr, CAIRO_COLOR(color));

    cairo_move_to(cr, offset_x, offset_y);
    pango_cairo_show_layout(cr, pango_layout);
}

BUBBLES_NAMESPACE_END
