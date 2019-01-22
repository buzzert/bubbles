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
    : _scale(1.0), _canvasRect(canvasRect)
{
    _surface = x11_helper_acquire_cairo_surface(canvasRect.width, canvasRect.height);
    _cr = cairo_create(_surface);

    // TODO: Need to do this on window size change too
    cairo_xlib_surface_set_size(_surface, canvasRect.width, canvasRect.height);

    if (!windowed) {
        x11_helper_set_fullscreen(true);
    }
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
    _scale = scale;
    // TODO: actually use this (cairo_scale before scene render call?)
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
    while (running) {
        struct timespec render_begin, render_end;
        clock_gettime(CLOCK_MONOTONIC_RAW, &render_begin);

        update();
        render();

        clock_gettime(CLOCK_MONOTONIC_RAW, &render_end);

        long render_time_nsec = (render_end.tv_nsec - render_begin.tv_nsec);
        struct timespec sleep_time = { 0, (sleep_nsec - render_time_nsec) };
        nanosleep(&sleep_time, NULL);
    }
}

BUBBLES_NAMESPACE_END
