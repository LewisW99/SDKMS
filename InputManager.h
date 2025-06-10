#pragma once
#include <SDL3/SDL.h>
#include <unordered_map>

class InputManager {
public:
    void handleEvent(const SDL_Event& event);
    void update();  // Call this once per frame

    // Query input states
    bool isKeyDown(SDL_Scancode key) const;
    bool isKeyJustPressed(SDL_Scancode key) const;
    bool isKeyJustReleased(SDL_Scancode key) const;

    // --- Semantic input helpers ---
    bool isMovingLeft() const;
    bool isMovingRight() const;
    bool isMovingUp() const;
    bool isMovingDown() const;
    bool isActionPressed() const;
    bool isPaused() const;

private:
    std::unordered_map<SDL_Scancode, bool> currentKeys;
    std::unordered_map<SDL_Scancode, bool> previousKeys;
};
