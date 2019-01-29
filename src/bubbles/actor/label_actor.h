/*
 * Created on Thu Dec 20 2018
 *
 * Copyright (c) 2018 James Magahern <james@magahern.com>
 */

#pragma once

#include <bubbles/actor/actor.h>
#include <bubbles/core/color.h>
#include <bubbles/core/foundation.h>

#include <pango/pangocairo.h>

BUBBLES_NAMESPACE_BEGIN

DEF_SHARED_PTR(LabelActor)

class LabelActor : public Actor
{
public:
    LabelActor(Rect rect, std::string contents);
    ~LabelActor() override;

    void set_font_prop(std::string font_prop);
    const std::string& get_font_prop() const;

    void set_contents(std::string contents);
    const std::string& get_contents() const;

    void set_foreground_color(Color c);
    const Color& get_foreground_color() const;

    void set_alignment(PangoAlignment alignment);
    const PangoAlignment& get_alignment() const;

    Size get_intrinsic_size() const;

    void render(cairo_t *cr, Rect at_rect);
private:
    std::string    _font_prop;
    std::string    _contents;
    Color          _foreground_color;
    PangoAlignment _alignment;

    bool           _pango_layout_valid = false;
    PangoLayout   *_pango_layout;

    PangoLayout* get_pango_layout(cairo_t *cr);
};

BUBBLES_NAMESPACE_END
