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
    MainScene(Rect canvasRect, bool windowed);
    ~MainScene();

    void add_actor(ActorPtr actor);
    void set_scale(float scale);
    void set_framerate(unsigned int frames_per_sec);

    void update();
    void render();

    /// Starts main run loop
    void run();

private:
    SDL_Window   *_window;
    SDL_Renderer *_renderer;

    std::vector<ActorPtr> _actors;

    Rect _canvasRect;
    unsigned int _frames_per_sec = 60;
};

BUBBLES_NAMESPACE_END
