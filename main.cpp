#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "GameApp.h"

static GameApp app;

SDL_AppResult SDL_AppInit(void**, int, char**) {
    return app.init() ? SDL_APP_CONTINUE : SDL_APP_FAILURE;
}

SDL_AppResult SDL_AppEvent(void*, SDL_Event* event) {
    static bool running = true;
    app.handleEvent(*event, running);
    return running ? SDL_APP_CONTINUE : SDL_APP_SUCCESS;
}

SDL_AppResult SDL_AppIterate(void*) {
    app.update();
    app.render();
    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void*, SDL_AppResult) {
    app.cleanup();  // also handled in ~GameApp(), but safe
}
