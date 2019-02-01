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

#define DEF_SHARED_PTR(type) \
    class type; \
    using type##Ptr = std::shared_ptr<type>;

DEF_SHARED_PTR(Actor)

class MainScene;
class Actor
{
public:
    Actor(Rect rect);
    virtual ~Actor() {};

    virtual void update();
    virtual void render(cairo_t *cr, Rect at_rect);
    virtual void clear(cairo_t *cr, Color color);

    void add_subactor(ActorPtr actor);
    void remove_subactor(ActorPtr actor);

    Rect get_rect() const;
    void set_rect(Rect r);

    Rect get_bounds() const { return Rect(0, 0, rect.width, rect.height); };

    float get_alpha() const { return _alpha; };
    void set_alpha(float alpha) { _alpha = alpha; };

    Color get_background_color() const { return _background_color; }
    void  set_background_color(Color c) { _background_color = c; }

    double get_scene_scale() const;

    void set_needs_display();

    bool interaction_enabled = false;

    virtual void layout_actors() {};

    // Events
    virtual void mouse_down(int x, int y);
    virtual void mouse_up(int x, int y);

private:
    friend class MainScene;
    MainScene *_parent_scene = nullptr;
    Actor     *_super_actor = nullptr;

    Actor* hit_test(int x, int y);

protected:
    Rect  rect;

    float _alpha = 1.0;
    Color _background_color = Colors::black;

    bool _needs_display = true;
    bool _needs_layout = true;

    std::vector<ActorPtr> _subactors;
    MainScene* get_parent_scene() const;
};

BUBBLES_NAMESPACE_END
