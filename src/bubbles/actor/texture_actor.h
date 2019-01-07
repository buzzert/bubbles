/*
 * Created on Thu Dec 20 2018
 *
 * Copyright (c) 2018 James Magahern <james@magahern.com>
 */

#pragma once

#include <bubbles/actor/actor.h>
#include <bubbles/core/foundation.h>

BUBBLES_NAMESPACE_BEGIN

std::shared_ptr<SDL_Texture> TextureForRes(std::string const &resourceName, SDL_Renderer *renderer);

class TextureActor : public Actor
{
public:
    TextureActor(Rect r);
    TextureActor(std::shared_ptr<SDL_Texture> texture);
    ~TextureActor() override {};

    void render(SDL_Renderer *renderer, Rect at_rect) override;

    std::shared_ptr<SDL_Texture> texture;
};

BUBBLES_NAMESPACE_END
