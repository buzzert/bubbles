/*
 * Created on Thu Dec 20 2018
 *
 * Copyright (c) 2018 James Magahern <james@magahern.com>
 */

#include "actor.h"

BUBBLES_NAMESPACE_BEGIN

Actor::Actor(Rect rect)
    : rect(rect), alpha(1.0), _needs_display(true), _background_color(Color(0xFF, 0xFF, 0xFF, 0x00))
{}

void Actor::set_rect(Rect r)
{
    rect = r;
    _needs_display = true;
}

Rect Actor::get_rect() const
{
    return rect;
}

void Actor::add_subactor(ActorPtr actor)
{
    _subactors.push_back(actor);
}

void Actor::remove_subactor(ActorPtr actor)
{
    // todo
}

void Actor::update()
{
    if (_needs_layout) {
        layout_actors();
        _needs_layout = false;
    }

    for (ActorPtr a : _subactors) {
        a->update();
    }
}

void Actor::render(cairo_t *cr, Rect at_rect)
{
    // Clear with background colorz
    _background_color.set_source(cr);
    cairo_rectangle(cr, 0.0, 0.0, rect.width, rect.height);
    cairo_fill(cr);

    // TODO: untested
    for (ActorPtr a : _subactors) {
        cairo_save(cr);

        Rect a_rect = a->get_rect();
        cairo_translate(cr, a_rect.x, a_rect.y);

        a->render(cr, rect);

        cairo_restore(cr);
    }
}

BUBBLES_NAMESPACE_END
