#include "InputManager.h"

void InputManager::handleEvent(const SDL_Event& event) {
    if (event.type == SDL_EVENT_KEY_DOWN) {
        currentKeys[event.key.scancode] = true;
    }
    else if (event.type == SDL_EVENT_KEY_UP) {
        currentKeys[event.key.scancode] = false;
        
    }
}

void InputManager::update() {
    previousKeys = currentKeys;  // Store previous frame state
}

bool InputManager::isKeyDown(SDL_Scancode key) const {
    return currentKeys.count(key) ? currentKeys.at(key) : false;
}

bool InputManager::isKeyJustPressed(SDL_Scancode key) const {
    return isKeyDown(key) && (!previousKeys.count(key) || !previousKeys.at(key));
}

bool InputManager::isKeyJustReleased(SDL_Scancode key) const {
    return !isKeyDown(key) && previousKeys.count(key) && previousKeys.at(key);
}

bool InputManager::isMovingLeft() const {
    return isKeyDown(SDL_SCANCODE_LEFT) || isKeyDown(SDL_SCANCODE_A);
}

bool InputManager::isMovingRight() const {
    return isKeyDown(SDL_SCANCODE_RIGHT) || isKeyDown(SDL_SCANCODE_D);
}

bool InputManager::isMovingUp() const {
    return isKeyDown(SDL_SCANCODE_UP) || isKeyDown(SDL_SCANCODE_W);
}

bool InputManager::isMovingDown() const {
    return isKeyDown(SDL_SCANCODE_DOWN) || isKeyDown(SDL_SCANCODE_S);
}

bool InputManager::isActionPressed() const {
    return isKeyJustPressed(SDL_SCANCODE_SPACE);
}

bool InputManager::isPaused() const {
    return isKeyJustPressed(SDL_SCANCODE_ESCAPE);
}