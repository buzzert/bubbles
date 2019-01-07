/*
 * Created on Thu Dec 20 2018
 *
 * Copyright (c) 2018 James Magahern <james@magahern.com>
 */

#pragma once

#include <bubbles/core/foundation.h>
#include <bubbles/core/geometry.h>

BUBBLES_NAMESPACE_BEGIN

#define DEFAULT_RGBA_MASK 0x00FF0000, 0x0000FF00, 0x000000FF, 0x00

class Actor;
using ActorPtr = std::shared_ptr<Actor>;

class Actor
{
public:
    Actor(Rect rect);
    virtual ~Actor() {};

    virtual void update(SDL_Renderer *renderer);
    virtual void render(SDL_Renderer *renderer, Rect at_rect);

    void add_subactor(ActorPtr actor);
    void remove_subactor(ActorPtr actor);

    Rect get_rect() const;
    void set_rect(Rect r);

    void set_needs_display() { _needs_display = true; };

    virtual void layout_actors() {};
protected:
    Rect  rect;
    float alpha;

    bool _needs_display = true;
    bool _needs_layout = true;

    std::vector<ActorPtr> _subactors;
};

BUBBLES_NAMESPACE_END
