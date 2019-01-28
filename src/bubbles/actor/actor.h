/*
 * Created on Thu Dec 20 2018
 *
 * Copyright (c) 2018 James Magahern <james@magahern.com>
 */

#pragma once

#include <bubbles/core/color.h>
#include <bubbles/core/foundation.h>
#include <bubbles/core/geometry.h>

BUBBLES_NAMESPACE_BEGIN

#define DEFAULT_RGBA_MASK 0x00FF0000, 0x0000FF00, 0x000000FF, 0x00

class Actor;
using ActorPtr = std::shared_ptr<Actor>;

class MainScene;
class Actor
{
public:
    Actor(Rect rect);
    virtual ~Actor() {};

    virtual void update();
    virtual void render(cairo_t *cr, Rect at_rect);

    void add_subactor(ActorPtr actor);
    void remove_subactor(ActorPtr actor);

    Rect get_rect() const;
    void set_rect(Rect r);

    Color get_background_color() const { return _background_color; }
    void  set_background_color(Color c) { _background_color = c; }

    double get_scene_scale() const;

    void set_needs_display() { _needs_display = true; };

    virtual void layout_actors() {};

    // Events
    virtual void mouse_down(int x, int y);
    virtual void mouse_up(int x, int y);

private:
    friend class MainScene;
    MainScene *_parent_scene;
    Actor     *_super_actor;

    Actor* hit_test(int x, int y);

protected:
    Rect  rect;
    float alpha;

    Color _background_color;

    bool _needs_display = true;
    bool _needs_layout = true;

    std::vector<ActorPtr> _subactors;
    MainScene* get_parent_scene() const;
};

BUBBLES_NAMESPACE_END
