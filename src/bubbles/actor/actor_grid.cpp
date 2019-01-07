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

void ActorGrid::update(SDL_Renderer *renderer)
{
    layout_if_needed();
    each_actor([&] (ActorPtr actor) {
        actor->update(renderer);
    });
}

void ActorGrid::render(SDL_Renderer *renderer, Rect at_rect)
{
    each_actor([&] (ActorPtr actor) {
        Rect rect = actor->get_rect();
        rect.x += at_rect.x;
        rect.y += at_rect.y;

        actor->render(renderer, rect);
    });
}

void ActorGrid::stack_actor(ActorPtr actor, unsigned atColumn, float height)
{
    GridItem item {
        .actor = actor,
        .requested_height = height
    };

    _grid[atColumn].push_back(item);
    _needs_layout = true;
}

void ActorGrid::layout_if_needed()
{
    if (!_needs_layout) return;

    // Currently columns are evenly spaced
    float column_width = rect.width / _cols;

    // Foreach col
    float x_offset = 0.0;
    for (std::vector<GridItem> col : _grid) {
        float flexible_space_remaining = rect.height;

        // Compute ahead of time: subtract grid items with defined height
        unsigned num_flexible_items = 0;
        for (GridItem &item : col) {
            if (item.requested_height > 0) {
                flexible_space_remaining -= item.requested_height;
            } else {
                num_flexible_items++;
            }
        }

        float y_offset = 0.0;
        float flexible_item_height = (flexible_space_remaining / num_flexible_items);
        for (GridItem &item : col) {
            Rect r;
            r.x = x_offset;
            r.y = y_offset;
            r.width = column_width;
            if (item.requested_height > 0) {
                r.height = item.requested_height;
            } else {
                r.height = flexible_item_height;
            }

            item.actor->set_rect(r);
            y_offset += r.height;
        }

        x_offset += column_width;
    }

    _needs_layout = false;
}

BUBBLES_NAMESPACE_END
