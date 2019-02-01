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
    : rect(rect)
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

void Actor::set_needs_display()
{
    _needs_display = true;
    
    // Go up the hierarchy to ensure all super actors also get needs_display
    Actor *super = _super_actor;
    while (super) {
        super->_needs_display = true;
        super = super->_super_actor;
    }

    // Also go down, since we need to redraw from here down
    for (ActorPtr a : _subactors) {
        a->set_needs_display();
    }
}

Actor* Actor::hit_test(int x, int y)
{
    Actor *actor = this;
    for (const ActorPtr a : _subactors) {
        Rect r = a->get_rect();
        if (r.point_inside(x, y)) {
            actor = a->hit_test(x - r.x, y - r.y);
            break;
        }
    }

    return actor;
}

void Actor::mouse_down(int x, int y)
{
    if (!interaction_enabled && _super_actor) {
        _super_actor->mouse_down(x, y);
    }
}

void Actor::mouse_up(int x, int y)
{
    if (!interaction_enabled && _super_actor) {
        _super_actor->mouse_up(x, y);
    }
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

void Actor::clear(cairo_t *cr, Color color)
{
    if (color.alpha == 0) return;

    cairo_save(cr);
        cairo_set_operator(cr, CAIRO_OPERATOR_SOURCE);
        color.set_source(cr);
        cairo_rectangle(cr, 0.0, 0.0, rect.width, rect.height);
        cairo_fill(cr);
    cairo_restore(cr);
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
    // Clear with background color
    if (_needs_display && _super_actor != nullptr) {
        clear(cr, _background_color);
    }

    // TODO: untested
    for (ActorPtr a : _subactors) {
        if (!a->_needs_display) continue;

        cairo_save(cr);

        Rect a_rect = a->get_rect();
        cairo_translate(cr, a_rect.x, a_rect.y);

        a->render(cr, rect);

        cairo_restore(cr);
    }

    _needs_display = false;
}

BUBBLES_NAMESPACE_END
