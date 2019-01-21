/*
 * Created on Sun Dec 30 2018
 *
 * Copyright (c) 2018 James Magahern <james@magahern.com>
 */

#include "actor_grid.h"

BUBBLES_NAMESPACE_BEGIN

ActorGrid::ActorGrid(Rect r, unsigned cols)
    : Actor(r), _cols(cols), _grid(cols)
{

}

ActorGrid::~ActorGrid()
{

}

void ActorGrid::set_orientation(ActorGrid::Orientation orientation)
{
    _orientation = orientation;
    _needs_layout = true;
}

void ActorGrid::each_grid_item(const GridItemIter &iter)
{
    for (unsigned col = 0; col < _cols; col++) {
        std::vector<GridItem> col_items = _grid[col];
        for (GridItem &item : col_items) {
            iter(col, item);
        }
    }
}

void ActorGrid::each_actor(const ActorIter &iter)
{
    each_grid_item([&] (unsigned col, GridItem item) {
        iter(item.actor);
    });
}

void ActorGrid::update()
{
    layout_if_needed();
    each_actor([&] (ActorPtr actor) {
        actor->update();
    });
}

void ActorGrid::render(cairo_t *cr, Rect at_rect)
{
    Actor::render(cr, at_rect);

    each_actor([&] (ActorPtr actor) {
        cairo_save(cr);

        Rect rect = actor->get_rect();
        cairo_translate(cr, rect.x, rect.y);

        actor->render(cr, rect);

        cairo_restore(cr);
    });
}

void ActorGrid::stack_actor(ActorPtr actor, unsigned atColumn, float size)
{
    GridItem item {
        .actor = actor,
        .requested_size = size
    };

    _grid[atColumn].push_back(item);
    _needs_layout = true;
}

float ActorGrid::variable_dimension(Rect &r) const
{
    if (Orientation::HORIZONTAL == _orientation) {
        return r.width;
    } else {
        return r.height;
    }
}

void ActorGrid::assign_variable_dimension(Rect &r, float value)
{
    if (Orientation::HORIZONTAL == _orientation) {
        r.width = value;
    } else {
        r.height = value;
    }
}

float ActorGrid::fixed_dimension(Rect &r) const
{
    if (Orientation::HORIZONTAL == _orientation) {
        return r.height;
    } else {
        return r.width;
    }
}

void ActorGrid::assign_fixed_dimension(Rect &r, float value)
{
    if (Orientation::HORIZONTAL == _orientation) {
        r.height = value;
    } else {
        r.width = value;
    }
}

void ActorGrid::layout_if_needed()
{
    if (!_needs_layout) return;

    // Currently columns are evenly spaced
    float fixed_item_size = fixed_dimension(rect) / _cols;

    // Foreach col
    Rect offset;
    for (std::vector<GridItem> col : _grid) {
        float flexible_space_remaining = variable_dimension(rect);

        // Compute ahead of time: subtract grid items with defined height
        unsigned num_flexible_items = 0;
        for (GridItem &item : col) {
            if (item.requested_size > 0) {
                flexible_space_remaining -= item.requested_size;
            } else {
                num_flexible_items++;
            }
        }

        if (Orientation::VERTICAL == _orientation) {
            offset.y = 0.0;
        } else {
            offset.x = 0.0;
        }

        float flexible_item_size = (flexible_space_remaining / num_flexible_items);
        for (GridItem &item : col) {
            Rect r;
            r.x = offset.x;
            r.y = offset.y;
            assign_fixed_dimension(r, fixed_item_size);
            if (item.requested_size > 0) {
                assign_variable_dimension(r, item.requested_size);
            } else {
                assign_variable_dimension(r, flexible_item_size);
            }

            item.actor->set_rect(r);

            if (Orientation::VERTICAL == _orientation) {
                offset.y += variable_dimension(r);
            } else {
                offset.x += variable_dimension(r);
            }
        }

        if (Orientation::VERTICAL == _orientation) {
            offset.x += fixed_item_size;
        } else {
            offset.y += fixed_item_size;
        }
    }

    _needs_layout = false;
}

BUBBLES_NAMESPACE_END
