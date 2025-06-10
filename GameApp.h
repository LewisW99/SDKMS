
#pragma once
#include <SDL3/SDL.h>
#include <string>
#include "InputManager.h"
#include "TextureManager.h"
#include "SceneManager.h"

class GameApp {
public:
    GameApp();
    ~GameApp();

    bool init();
    void handleEvent(const SDL_Event& event, bool& running);
    void update();
    void render();
    void cleanup();

private:
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    std::string message = "Hello World!";

private:
    InputManager input;
    TextureManager textures;
    SceneManager sceneManager;
};
