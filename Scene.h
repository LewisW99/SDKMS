#pragma once
#include <SDL3/SDL.h>

class Scene {
public:
    virtual ~Scene() = default;

    virtual void handleEvent(const SDL_Event& event) = 0;
    virtual void update() = 0;
    virtual void render(SDL_Renderer* renderer) = 0;
};
