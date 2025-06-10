#pragma once
#include "Scene.h"
#include "SceneManager.h"
#include "InputManager.h"
#include <string>

class TitleScene : public Scene {
public:
    TitleScene(SceneManager& manager, SDL_Renderer* renderer);

    void handleEvent(const SDL_Event& event) override;
    void update() override;
    void render(SDL_Renderer* renderer) override;

private:
    SceneManager& sceneManager;
    SDL_Renderer* renderer = nullptr;  
    std::string title = "PRESS SPACE TO START";
};
