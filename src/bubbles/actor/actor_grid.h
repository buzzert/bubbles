/*
 * Created on Sun Dec 30 2018
 *
 * Copyright (c) 2018 James Magahern <james@magahern.com>
 */

#pragma once

#include <functional>
#include <bubbles/actor/actor.h>
#include <bubbles/core/foundation.h>

BUBBLES_NAMESPACE_BEGIN

class ActorGrid : public Actor
{
public:
    enum Orientation {
        HORIZONTAL,
        VERTICAL
    };

    ActorGrid(Rect r, unsigned cols);
    ~ActorGrid() override;

    void update() override;
    void render(cairo_t *cr, Rect at_rect) override;

    /// Use size = -1 for "automatic" 
    void stack_actor(ActorPtr actor, unsigned atColumn, float size = -1);

    void set_orientation(ActorGrid::Orientation orientation);

private:
    struct GridItem {
        ActorPtr actor;
        float    requested_size;
            // that is, requested_size in the variable dimension
    };

    bool     _needs_layout = true;
    unsigned _cols;

    ActorGrid::Orientation _orientation = ActorGrid::Orientation::VERTICAL;
    std::vector<std::vector<GridItem>> _grid;

    using ActorIter = std::function<void(ActorPtr)>;
    void each_actor(const ActorIter &iter);

    using GridItemIter = std::function<void(unsigned, GridItem)>;
    void each_grid_item(const GridItemIter &iter);

    float variable_dimension(Rect &r) const;
    void assign_variable_dimension(Rect &r, float value);

    float fixed_dimension(Rect &r) const;
    void assign_fixed_dimension(Rect &r, float value);

    void layout_if_needed();
};

BUBBLES_NAMESPACE_END
