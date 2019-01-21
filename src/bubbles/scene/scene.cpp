/*
 * Created on Thu Dec 20 2018
 *
 * Copyright (c) 2018 James Magahern <james@magahern.com>
 */

#include "scene.h"

extern "C" {
#include <bubbles/core/x11_support.h>
}

BUBBLES_NAMESPACE_BEGIN

MainScene::MainScene(Rect canvasRect, bool windowed)
    : _canvasRect(canvasRect)
{
    _surface = x11_helper_acquire_cairo_surface(canvasRect.width, canvasRect.height);
    _cr = cairo_create(_surface);

    // TODO: Need to do this on window size change too
    cairo_xlib_surface_set_size(_surface, canvasRect.width, canvasRect.height);
}

MainScene::~MainScene()
{
    cairo_destroy(_cr);
    cairo_surface_destroy(_surface);
}

void MainScene::poll_events()
{
    XEvent e;

    // Temp: this should be handled by x11_support
    Display *display = cairo_xlib_surface_get_display(_surface);
    for (;;) {
        if (XPending(display)) {
            // XNextEvent blocks the caller until an event arrives
            XNextEvent(display, &e);
        } else {
            return;
        }

        switch (e.type) {
            case ConfigureNotify:
                // TODO
                break;
            case ButtonPress:
            case KeyPress:
            default:
                break;
        }
    }
}

void MainScene::update()
{
    poll_events();

    for (auto &a : _actors) {
        a->update();
    }
}

void MainScene::add_actor(ActorPtr a)
{
    _actors.push_back(a);
}

void MainScene::set_scale(float scale)
{
    // TODO: set_scale
    //SDL_RenderSetScale(_renderer, scale, scale);
}

void MainScene::set_framerate(unsigned int fps)
{
    _frames_per_sec = fps;
}

void MainScene::render()
{
    cairo_push_group(_cr);

    cairo_set_source_rgba(_cr, 0.0, 0.0, 0.0, 1.0);
    cairo_paint(_cr);

    for (ActorPtr &a : _actors) {
        Rect rect = a->get_rect();
        a->render(_cr, rect);
    }

    cairo_pop_group_to_source(_cr);

    cairo_paint(_cr);
    cairo_surface_flush(_surface);
}

void MainScene::run()
{
    bool running = true;

    const int frames_per_sec = 60;
    const long sleep_nsec = (1.0 / frames_per_sec) * 1000000000;
    struct timespec sleep_time = { 0, sleep_nsec };
    while (running) {
        update();
        render();

        // TODO: sleep for less time if it took more than 13.33ms to update frame
        nanosleep(&sleep_time, NULL);
    }
}

BUBBLES_NAMESPACE_END
