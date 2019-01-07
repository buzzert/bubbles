/*
 * Created on Thu Dec 20 2018
 *
 * Copyright (c) 2018 James Magahern <james@magahern.com>
 */

#include "actor.h"

BUBBLES_NAMESPACE_BEGIN

Actor::Actor(Rect rect)
    : rect(rect), alpha(1.0), _needs_display(true)
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

void Actor::update(SDL_Renderer *renderer)
{
    if (_needs_layout) {
        layout_actors();
        _needs_layout = false;
    }

    for (ActorPtr a : _subactors) {
        a->update(renderer);
    }
}

void Actor::render(SDL_Renderer *renderer, Rect at_rect)
{
    for (ActorPtr a : _subactors) {
        Rect rect = a->get_rect();
        rect.x += at_rect.x;
        rect.y += at_rect.y;

        a->render(renderer, rect);
    }
}

BUBBLES_NAMESPACE_END
