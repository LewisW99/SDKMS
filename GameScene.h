#pragma once
#include "Scene.h"
#include "InputManager.h"
#include <SDL3/SDL.h>
#include <cmath>
#include <vector>
#include <utility>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

struct Pickup {
    float x, y;
    bool collected = false;
};


class GameScene : public Scene {
public:
    GameScene(SDL_Renderer* renderer);
    void handleEvent(const SDL_Event& event) override;
    void update() override;
    void render(SDL_Renderer* renderer) override;

private:
    InputManager input;
    SDL_Renderer* renderer = nullptr;

    // Player state
    float playerX = 4.5f;
    float playerY = 4.5f;
    float speed = 0.1f; // units per second

    float playerAngle = 0.0f;
    const float FOV = 60.0f * (M_PI / 180.0f);

    // Map
    static const int mapWidth = 10;
    static const int mapHeight = 10;
    int map[mapHeight][mapWidth] = {
        {1,1,1,1,1,1,1,1,1,1},
        {1,0,0,0,0,0,0,0,0,1},
        {1,0,1,0,1,0,1,0,0,1},
        {1,0,0,0,0,0,0,1,0,1},
        {1,1,1,1,1,1,0,1,0,1},
        {1,0,0,0,0,0,0,1,0,1},
        {1,0,1,1,1,1,1,1,0,1},
        {1,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,1},
        {1,1,1,1,1,1,1,1,1,1}
    };

    int lightMap[mapHeight][mapWidth] = {
    {2,2,2,2,2,2,2,2,2,2},
    {2,1,1,1,0,0,1,1,1,2},
    {2,1,2,1,2,1,2,1,1,2},
    {2,1,1,1,1,1,1,2,1,2},
    {2,2,2,2,2,2,1,2,1,2},
    {2,1,1,1,1,1,1,2,1,2},
    {2,1,2,2,2,2,2,2,1,2},
    {2,1,1,1,0,0,1,1,1,2},
    {2,1,1,1,1,1,1,1,1,2},
    {2,2,2,2,2,2,2,2,2,2}
    };


    void drawMap(SDL_Renderer* renderer);
    void drawPlayer(SDL_Renderer* renderer);
	void renderWallsRaycast(SDL_Renderer* renderer);
    

    float bobPhase = 0.1f;         // for smooth oscillation
    float bobAmount = 0.5f;        // current bob offset
    const float BOB_SPEED = 0.5f; // oscillation speed
    const float BOB_HEIGHT = 3.5f; // how far to move the view
    bool isWalking = false;

    std::vector<std::pair<int, int>> flickeringLights;
    uint64_t lastFlickerTime = 0;


    void renderPickups(SDL_Renderer* renderer);
    std::vector<Pickup> pickups;

    int pickupsCollected = 0;
    bool nearPickup = false;

    bool scaryEventTriggered = false;



};
