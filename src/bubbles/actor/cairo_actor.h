/*
 * Created on Sat Dec 22 2018
 *
 * Copyright (c) 2018 James Magahern <james@magahern.com>
 */

#pragma once

#include <bubbles/actor/texture_actor.h>
#include <bubbles/core/foundation.h>
#include <cairo/cairo.h>

BUBBLES_NAMESPACE_BEGIN

class CairoActor : public TextureActor 
{
public:
    CairoActor(Rect r);

    void render(SDL_Renderer *renderer, Rect at_rect) override;

    // This is called when the cairo context needs to be redrawn. Independent from rendering,
    // which simply blits the cairo surface to the SDL renderer. 
    virtual void display_surface();
protected:
    Rect                         _surface_bounds;
    std::shared_ptr<cairo_t>     _cairo_ctx;
    std::shared_ptr<SDL_Surface> _surface;

    void generate_surface();
};

BUBBLES_NAMESPACE_END
