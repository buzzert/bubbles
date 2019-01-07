/*
 * Created on Thu Dec 20 2018
 *
 * Copyright (c) 2018 James Magahern <james@magahern.com>
 */

#include "scene.h"

BUBBLES_NAMESPACE_BEGIN

MainScene::MainScene(Rect canvasRect, bool windowed)
    : _canvasRect(canvasRect)
{
    int flags = windowed ? SDL_WINDOW_SHOWN : SDL_WINDOW_FULLSCREEN_DESKTOP;

    _window = SDL_CreateWindow("", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, canvasRect.width, canvasRect.height, flags);
    if (_window) {
        _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    }
}

MainScene::~MainScene()
{
    SDL_DestroyRenderer(_renderer);
    SDL_DestroyWindow(_window);
}

void MainScene::update()
{
    for (auto &a : _actors) {
        a->update(_renderer);
    }
}

void MainScene::add_actor(ActorPtr a)
{
    _actors.push_back(a);
}

void MainScene::set_scale(float scale)
{
    SDL_RenderSetScale(_renderer, scale, scale);
}

void MainScene::set_framerate(unsigned int fps)
{
    _frames_per_sec = fps;
}

void MainScene::render()
{
    SDL_SetRenderDrawColor(_renderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(_renderer);

    for (ActorPtr &a : _actors) {
        Rect rect = a->get_rect();
        a->render(_renderer, rect);
    }

    SDL_RenderPresent(_renderer);
}

void MainScene::run()
{
    bool running = true;
    const int kTicksPerFrame = 1000 / _frames_per_sec;

    while (running) {
        Uint32 startTime = SDL_GetTicks();

        SDL_Event e;
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                running = false;
            }
        }

        update();
        render();

        Uint32 ticks = SDL_GetTicks() - startTime;
        if (ticks < kTicksPerFrame) {
            SDL_Delay(kTicksPerFrame - ticks);
        }
    }
}

BUBBLES_NAMESPACE_END
