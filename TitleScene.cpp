#include "TitleScene.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_log.h>
#include "GameScene.h"



TitleScene::TitleScene(SceneManager& manager, SDL_Renderer* renderer)
    : sceneManager(manager) {
}

void TitleScene::handleEvent(const SDL_Event& event) {

}

void TitleScene::update() {

    
}

void TitleScene::render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDebugText(renderer, 100, 100, title.c_str());

    SDL_RenderPresent(renderer);
}
