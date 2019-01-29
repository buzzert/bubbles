/*
 * Created on Thu Dec 20 2018
 *
 * Copyright (c) 2018 James Magahern <james@magahern.com>
 */

#pragma once

#include <bubbles/actor/actor.h>
#include <bubbles/core/foundation.h>
#include <chrono>

BUBBLES_NAMESPACE_BEGIN

typedef std::chrono::high_resolution_clock::time_point update_time;

class MainScene
{
public:
    MainScene(Rect canvasRect, bool windowed, double scale = 1.0);
    ~MainScene();

    void add_actor(ActorPtr actor);

    void set_scale(float scale);
    double get_scale() const { return _scale; };

    void set_framerate(unsigned int frames_per_sec);
    void set_hides_cursor(bool hides_cursor);

    void update();
    void render();

    /// Starts main run loop
    void run();

private:
    cairo_t         *_cr;
    cairo_surface_t *_surface;

    double           _scale;

    Actor _primary_actor;

    Rect _canvasRect;
    unsigned int _frames_per_sec = 60;

    Actor *_tracked_actor = nullptr;
    void pointer_event(int x, int y, bool pressed);
    static void handle_pointer_callback(void *context, int x, int y, bool pressed);
};

BUBBLES_NAMESPACE_END
