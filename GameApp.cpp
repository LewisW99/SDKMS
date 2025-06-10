// GameApp.cpp
#include "GameApp.h"
#include "TitleScene.h"
#include "GameScene.h"

GameApp::GameApp() {}

GameApp::~GameApp() {
    cleanup();
}

bool GameApp::init() {
    if (!SDL_CreateWindowAndRenderer("Hello World", 800, 600, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
        SDL_Log("Couldn't create window and renderer: %s", SDL_GetError());
        return false;
    }

    sceneManager.setScene(std::make_unique<TitleScene>(sceneManager, renderer));


    textures.loadTexture(renderer, "player", "assets/player.bmp");
    return true;
}

void GameApp::handleEvent(const SDL_Event& event, bool& running) {
    input.handleEvent(event);

    sceneManager.handleEvent(event);

    if (event.type == SDL_EVENT_QUIT ) {
        running = false;
    }
}

void GameApp::update() {
    input.update();
    sceneManager.update();

    if (input.isPaused()) {
        SDL_Log("Game paused or exit requested");
        SDL_Event quit;
        quit.type = SDL_EVENT_QUIT;
        SDL_PushEvent(&quit);
    }
    if (input.isKeyDown(SDL_SCANCODE_SPACE)) {
        SDL_Log("Switching to GameScene...");
        sceneManager.setScene(std::make_unique<GameScene>(renderer));
    }

    if (input.isMovingLeft()) SDL_Log("Moving left");
    if (input.isActionPressed()) SDL_Log("Action button pressed!");
}

void GameApp::render() {
   /* int w = 0, h = 0;
    float scale = 4.0f;
    float x, y;*/


    //SDL_GetRenderOutputSize(renderer, &w, &h);
    //SDL_SetRenderScale(renderer, scale, scale);
    //x = ((w / scale) - SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE * message.size()) / 2;
    //y = ((h / scale) - SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE) / 2;

    //SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    sceneManager.render(renderer);
    //SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    //SDL_RenderDebugText(renderer, x, y, message.c_str());
    //SDL_Texture* tex = textures.getTexture("player");
    //if (tex) {
    //    SDL_Rect dest = { 100, 100, 64, 64 };  // You can adjust size or use SDL_QueryTexture
    //    SDL_FRect fDest = { static_cast<float>(dest.x), static_cast<float>(dest.y), static_cast<float>(dest.w), static_cast<float>(dest.h) };
    //    SDL_RenderTexture(renderer, tex, nullptr, &fDest);
    //}
    SDL_RenderPresent(renderer);


}

void GameApp::cleanup() {
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    textures.unloadAll();
}
