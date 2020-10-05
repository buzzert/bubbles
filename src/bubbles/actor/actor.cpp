/*
 * Created on Thu Dec 20 2018
 *
 * Copyright (c) 2018 James Magahern <james@magahern.com>
 */

#include "actor.h"

#include <algorithm>
#include <bubbles/scene/scene.h>

#include <cstdlib>
#include <unistd.h>

BUBBLES_NAMESPACE_BEGIN

Actor::Actor(Rect rect)
    : rect(rect)
{}

void Actor::set_rect(Rect r)
{
    rect = r;
    set_needs_layout();
    set_needs_display();
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

cairo_t* Actor::get_inherited_cairo_context() const
{
    MainScene *parent_scene = get_parent_scene();
    if (!parent_scene) {
        return nullptr;
    }

    return parent_scene->get_cairo_context();
}

void Actor::set_needs_display()
{
    set_needs_display(true);
}

void Actor::set_needs_display(bool notify_super)
{
    _needs_display = true;
    
    // Go up the hierarchy to ensure all super actors also get needs_display
    if (notify_super) {
        Rect damage_rect = rect;
        Actor *super = _super_actor;
        while (super) {
            super->_damage_rect = damage_rect;
            super->_needs_display = true;
            super = super->_super_actor;

            damage_rect = Rect(
                rect.x + _damage_rect.x,
                rect.y + _damage_rect.y,
                _damage_rect.width, _damage_rect.height
            );
        }
    }

    // Also go down, since we need to redraw from here down
    for (ActorPtr a : _subactors) {
        a->set_needs_display(false);
    }
}

void Actor::set_needs_layout()
{
    _needs_layout = true;
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
    
    Rect clear_rect = get_bounds();
    if (_damage_rect != RECT_ZERO) {
        clear_rect = _damage_rect;
    }

    cairo_save(cr);
        cairo_set_operator(cr, CAIRO_OPERATOR_SOURCE);
        color.set_source(cr);
        cairo_rectangle(cr, clear_rect.x, clear_rect.y, clear_rect.width, clear_rect.height);
        cairo_fill(cr);
    cairo_restore(cr);

    _damage_rect = RECT_ZERO;
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

    // Recursively render all subactors
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
