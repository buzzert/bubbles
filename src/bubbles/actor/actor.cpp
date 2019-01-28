/*
 * Created on Thu Dec 20 2018
 *
 * Copyright (c) 2018 James Magahern <james@magahern.com>
 */

#include "actor.h"

#include <algorithm>
#include <bubbles/scene/scene.h>

BUBBLES_NAMESPACE_BEGIN

Actor::Actor(Rect rect)
    : rect(rect), alpha(1.0), _background_color(Color(0xFF, 0xFF, 0xFF, 0x00)),
      _parent_scene(nullptr), _super_actor(nullptr), _needs_display(true)
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

double Actor::get_scene_scale() const
{
    double scale = 1.0;
    MainScene *scene = get_parent_scene();
    if (scene) {
        scale = scene->get_scale();
    }

    return scale;
}

MainScene* Actor::get_parent_scene() const
{
    if (_parent_scene == nullptr && _super_actor) {
        return _super_actor->get_parent_scene();
    }

    return _parent_scene;
}

Actor* Actor::hit_test(int x, int y)
{
    Actor *actor = nullptr;
    if (rect.point_inside(x, y)) {
        actor = this;
    }

    for (const ActorPtr a : _subactors) {
        Rect r = a->get_rect();
        if (r.point_inside(x, y)) {
            actor = a->hit_test(x - rect.x, y - rect.y);
            break;
        }
    }

    return actor;
}

void Actor::mouse_down(int x, int y)
{
}

void Actor::mouse_up(int x, int y)
{
}

void Actor::add_subactor(ActorPtr actor)
{
    _subactors.push_back(actor);
    actor->_super_actor = this;
}

void Actor::remove_subactor(ActorPtr actor)
{
    actor->_super_actor = nullptr;
    _subactors.erase(std::remove(_subactors.begin(), _subactors.end(), actor), _subactors.end());
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
