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

void MainScene::handle_pointer_callback(void *context, int x, int y, bool pressed)
{
    static_cast<MainScene *>(context)->pointer_event(x, y, pressed);
}

void MainScene::handle_window_delete_callback(void *context)
{
    static_cast<MainScene *>(context)->_running = false;
}

void MainScene::handle_window_expose_callback(void *context)
{
    static_cast<MainScene *>(context)->_primary_actor.set_needs_display();
}

MainScene::MainScene(Rect canvasRect, bool windowed, double scale)
    : _primary_actor(canvasRect), _canvasRect(canvasRect)
{
    _scale = scale;
    _surface = x11_helper_acquire_cairo_surface(canvasRect.width * scale, canvasRect.height * scale);
    _cr = cairo_create(_surface);

    // TODO: Need to do this on window size change too
    cairo_xlib_surface_set_size(_surface, canvasRect.width * scale, canvasRect.height * scale);

    // Typically the rule is container actors have transparent backgrounds, while actual actors
    // have opaque backgrounds
    _primary_actor.set_background_color(Colors::transparent);
    _primary_actor._parent_scene = this;

    if (!windowed) {
        x11_helper_set_fullscreen(true);
    }

    x11_callbacks_t callbacks = {
        .context = this,

        .pointer_callback = &MainScene::handle_pointer_callback,
        .window_delete_callback = &MainScene::handle_window_delete_callback,
        .window_expose_callback = &MainScene::handle_window_expose_callback,
    };

    x11_register_callbacks(callbacks);
}

MainScene::~MainScene()
{
    cairo_destroy(_cr);
    cairo_surface_destroy(_surface);
}

void MainScene::update()
{
    x11_poll_events();
    _primary_actor.update();
}

void MainScene::add_actor(ActorPtr a)
{
    _primary_actor.add_subactor(a);
}

void MainScene::set_scale(float scale)
{
    _scale = scale;
}

void MainScene::set_framerate(unsigned int fps)
{
    _frames_per_sec = fps;
}

void MainScene::set_hides_cursor(bool hides_cursor)
{
    x11_set_cursor_visible(!hides_cursor);
}

void MainScene::pointer_event(int x, int y, bool pressed)
{
    x /= _scale;
    y /= _scale;

    if (pressed) {
        Actor *hit_actor = _primary_actor.hit_test(x, y);
        if (hit_actor != nullptr) {
            _tracked_actor = hit_actor;
            hit_actor->mouse_down(x, y);
        }
    } else {
        // TODO: unsafe keeping this pointer around
        if (_tracked_actor) {
            // Convert point
            Actor *test_actor = _tracked_actor->_super_actor;
            while (test_actor != nullptr) {
                x -= test_actor->rect.x;
                y -= test_actor->rect.y;

                test_actor = test_actor->_super_actor;
            }

            _tracked_actor->mouse_up(x, y);
            _tracked_actor = nullptr;
        }
    }

}

void MainScene::render()
{
    cairo_push_group(_cr);
    if (_scale > 1.0) {
        cairo_scale(_cr, _scale, _scale);
    }

    _primary_actor.render(_cr, _canvasRect);

    cairo_pop_group_to_source(_cr);

    cairo_paint(_cr);
    cairo_surface_flush(_surface);
}

void MainScene::run()
{
    _running = true;

    const int frames_per_sec = 60;
    const long sleep_nsec = (1.0 / frames_per_sec) * 1000000000;
    while (_running) {
        update();
        render();

        struct timespec sleep_time = { 0, sleep_nsec };
        nanosleep(&sleep_time, NULL);
    }
}

BUBBLES_NAMESPACE_END
