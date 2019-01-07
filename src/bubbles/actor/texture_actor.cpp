/*
 * Created on Thu Dec 20 2018
 *
 * Copyright (c) 2018 James Magahern <james@magahern.com>
 */

#include "texture_actor.h"

#include <iostream>
#include <SDL2/SDL_image.h>

BUBBLES_NAMESPACE_BEGIN

std::shared_ptr<SDL_Texture> TextureForRes(std::string const &resourceName, SDL_Renderer *renderer)
{
    // TODO: resources (use gresources?)
    std::string const fullResourceName(resourceName);

    SDL_Surface *imageSurface = IMG_Load(fullResourceName.c_str());
    if (!imageSurface) {
        std::cerr << "WARN: Could not find resource: " << fullResourceName << std::endl;
        return NULL;
    }

    SDL_Texture *imageTexture = SDL_CreateTextureFromSurface(renderer, imageSurface);
    SDL_FreeSurface(imageSurface);

    return std::shared_ptr<SDL_Texture>(imageTexture, SDL_DestroyTexture);
}

TextureActor::TextureActor(Rect r)
    : Actor(r)
{
}

TextureActor::TextureActor(std::shared_ptr<SDL_Texture> texture)
    : Actor(Rect(0, 0, 0, 0)), texture(texture)
{
    int width, height;
    SDL_QueryTexture(texture.get(), NULL, NULL, &width, &height);

    rect.width = width;
    rect.height = height;
}

void TextureActor::render(SDL_Renderer *renderer, Rect at_rect)
{
    Actor::render(renderer, at_rect);
    
    SDL_Rect dst_rect = at_rect.to_sdl_rect();

    SDL_SetTextureAlphaMod(texture.get(), (alpha * 255));
    SDL_SetTextureBlendMode(texture.get(), SDL_BLENDMODE_ADD);
    SDL_RenderCopy(renderer, texture.get(), NULL, &dst_rect);
}

BUBBLES_NAMESPACE_END
