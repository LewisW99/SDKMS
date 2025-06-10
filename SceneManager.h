#pragma once
#include "Scene.h"
#include <memory>

class SceneManager {
public:
    void setScene(std::unique_ptr<Scene> newScene);
    void handleEvent(const SDL_Event& event);
    void update();
    void render(SDL_Renderer* renderer);

private:
    std::unique_ptr<Scene> currentScene;
};
