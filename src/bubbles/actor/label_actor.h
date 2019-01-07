/*
 * Created on Thu Dec 20 2018
 *
 * Copyright (c) 2018 James Magahern <james@magahern.com>
 */

#pragma once 

#include <bubbles/core/color.h>
#include <bubbles/core/foundation.h>
#include <bubbles/actor/cairo_actor.h>

#include <pango/pangocairo.h>

BUBBLES_NAMESPACE_BEGIN

class LabelActor : public CairoActor 
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

    void display_surface() override;
private:
    std::string    _font_prop;
    std::string    _contents;
    Color          _foreground_color;
    PangoAlignment _alignment;
};

BUBBLES_NAMESPACE_END
