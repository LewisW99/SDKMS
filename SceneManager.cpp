#include "SceneManager.h"

void SceneManager::setScene(std::unique_ptr<Scene> newScene) {
    currentScene = std::move(newScene);
}

void SceneManager::handleEvent(const SDL_Event& event) {
    if (currentScene) currentScene->handleEvent(event);
}

void SceneManager::update() {
    if (currentScene) currentScene->update();
}

void SceneManager::render(SDL_Renderer* renderer) {
    if (currentScene) currentScene->render(renderer);
}
