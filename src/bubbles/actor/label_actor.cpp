/*
 * Created on Thu Dec 20 2018
 *
 * Copyright (c) 2018 James Magahern <james@magahern.com>
 */

#include "label_actor.h"

BUBBLES_NAMESPACE_BEGIN

#define DEFAULT_FONT_PROP "Karla 24"

LabelActor::LabelActor(Rect rect, std::string contents)
    : Actor(rect), _contents(contents),
      _foreground_color(0xFF, 0xFF, 0xFF, 0xFF),
      _font_prop(DEFAULT_FONT_PROP),
      _alignment(PANGO_ALIGN_LEFT)
{
}

LabelActor::~LabelActor()
{

}

void LabelActor::set_contents(std::string contents)
{
    _contents = contents;
    set_needs_display();
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
    _font_prop = font_prop;
    set_needs_display();
}

const std::string& LabelActor::get_font_prop() const
{
    return _font_prop;    
}

void LabelActor::set_alignment(PangoAlignment alignment)
{
    _alignment = alignment;
    set_needs_display();
}

const PangoAlignment& LabelActor::get_alignment() const
{
    return _alignment;
}

void LabelActor::render(cairo_t *cr, Rect at_rect)
{
    Actor::render(cr, at_rect);

    PangoLayout *pango_layout = pango_cairo_create_layout(cr);

    PangoFontDescription *pango_desc = pango_font_description_from_string(_font_prop.c_str());
    pango_layout_set_font_description(pango_layout, pango_desc);
    pango_font_description_free(pango_desc);
    
    pango_layout_set_markup(pango_layout, _contents.c_str(), -1);

    // This actually only really affects how multiple lines work
    // If we're trying to draw text centered, we actually need to do calculation
    pango_layout_set_alignment(pango_layout, _alignment);

    int layout_width, layout_height;
    pango_layout_get_pixel_size(pango_layout, &layout_width, &layout_height);

    double offset_x = 0.0;
    double offset_y = 0.0;
    if (_alignment == PANGO_ALIGN_CENTER) {
        offset_x = (rect.width - layout_width) / 2.0;
    }

    // Maybe make this an option, but for now always center vertically
    offset_y = (rect.height - layout_height) / 2.0;

    Color &color = _foreground_color;
    cairo_set_source_rgba(cr, CAIRO_COLOR(color));

    cairo_move_to(cr, offset_x, offset_y);
    pango_cairo_show_layout(cr, pango_layout);
    g_object_unref(pango_layout);
}

BUBBLES_NAMESPACE_END
