#include "GameScene.h"
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_render.h>
#include <algorithm>
#include <SDL3/SDL_audio.h>
#include <SDL3/SDL_loadso.h>
#include <SDL3/SDL_filesystem.h>
#include <filesystem>

GameScene::GameScene(SDL_Renderer* renderer_)
    : renderer(renderer_) {
    playerX = 1.5f;
    playerY = 1.5f;

    flickeringLights.push_back({ 3, 4 });
    flickeringLights.push_back({ 6, 2 });
    flickeringLights.push_back({ 1, 7 });


    pickups.push_back({ 5.5f, 4.5f });
    pickups.push_back({ 8.0f, 2.0f });
    pickups.push_back({ 3.0f, 6.0f });



}

void GameScene::handleEvent(const SDL_Event& event) {
    input.handleEvent(event);
    nearPickup = false;

    if (event.type == SDL_EVENT_KEY_DOWN && event.key.scancode == SDL_SCANCODE_E) {

        for (auto& p : pickups) {
            if (p.collected) continue;

            float dx = playerX - p.x;
            float dy = playerY - p.y;
            float distSq = dx * dx + dy * dy;

            if (distSq < 1.0f) {
                p.collected = true;
                pickupsCollected++;
                SDL_Log("Pickup collected!");
            }
        }
    }
}

void GameScene::update() {

    uint64_t lastUpdateTime = 0;
    SDL_Log("Start tile: %d", map[static_cast<int>(playerY)][static_cast<int>(playerX)]);

    uint64_t now = SDL_GetTicks();  // milliseconds
    float deltaTime = (lastUpdateTime > 0) ? (now - lastUpdateTime) / 1000.0f : 0.016f;
    lastUpdateTime = now;

    input.update();

    float moveStep = speed * deltaTime;

    float nextX = playerX;
    float nextY = playerY;

    float dx = cosf(playerAngle);
    float dy = sinf(playerAngle);

    if (input.isMovingUp()) {
        nextX += dx * moveStep;
        nextY += dy * moveStep;
    }
    if (input.isMovingDown()) {
        nextX -= dx * moveStep;
        nextY -= dy * moveStep;
    }


    // Collision check
    int tileX = static_cast<int>(nextX);
    int tileY = static_cast<int>(nextY);

    if (tileX >= 0 && tileX < mapWidth &&
        tileY >= 0 && tileY < mapHeight &&
        map[tileY][tileX] == 0) {
        playerX = nextX;
        playerY = nextY;
    }

    if (input.isKeyDown(SDL_SCANCODE_LEFT))  playerAngle -= 0.5f * deltaTime;
    if (input.isKeyDown(SDL_SCANCODE_RIGHT)) playerAngle += 0.5f * deltaTime;

    SDL_Log("Player at (%.2f, %.2f) = tile [%d][%d]",
        playerX, playerY,
        static_cast<int>(playerY), static_cast<int>(playerX));

    isWalking = input.isMovingUp() || input.isMovingDown();

    if (isWalking) {
        bobPhase += deltaTime * BOB_SPEED;
        bobAmount = sinf(bobPhase) * BOB_HEIGHT;
    }
    else {
        bobPhase = 0;
        bobAmount = 0;
    }


    if (now - lastFlickerTime > 200) {
        lastFlickerTime = now;

        for (auto& pos : flickeringLights) {
            int x = pos.first;
            int y = pos.second;

            // Randomly flicker between 0 (off) and 2 (bright)
            int flicker = (rand() % 3);  // 0, 1, or 2
            lightMap[y][x] = flicker;
        }
    }
}

void GameScene::renderWallsRaycast(SDL_Renderer* renderer) {
    const int screenWidth = 800;
    const int screenHeight = 600;

    float viewOffsetY = bobAmount;
    const float rayStep = FOV / screenWidth;

    // --- Ceiling ---
    SDL_FRect ceiling = {
        0, 0 + viewOffsetY,
        static_cast<float>(screenWidth),
        static_cast<float>(screenHeight) / 2.0f
    };
    SDL_SetRenderDrawColor(renderer, 30, 30, 60, 255);  // dark blue/gray ceiling
    SDL_RenderFillRect(renderer, &ceiling);

    // --- Floor ---
    SDL_FRect floor = {
        0, screenHeight / 2.0f + viewOffsetY,
        static_cast<float>(screenWidth),
        static_cast<float>(screenHeight) / 2.0f
    };
    SDL_SetRenderDrawColor(renderer, 20, 15, 10, 255);  // dark floor
    SDL_RenderFillRect(renderer, &floor);

    for (int x = 0; x < screenWidth; ++x) {
        float rayAngle = playerAngle - (FOV / 2.0f) + (x * rayStep);
        float rayX = cosf(rayAngle);
        float rayY = sinf(rayAngle);

        float dist = 0.0f;
        const float maxDist = 20.0f;
        const float stepSize = 0.05f;

        while (dist < maxDist) {
            float testX = playerX + rayX * dist;
            float testY = playerY + rayY * dist;

            int tileX = static_cast<int>(testX);
            int tileY = static_cast<int>(testY);

            if (tileX < 0 || tileY < 0 || tileX >= mapWidth || tileY >= mapHeight) break;
            if (map[tileY][tileX] == 1) break;

            dist += stepSize;
        }

        float sliceHeight = (1.0f / dist) * 500.0f;
        if (sliceHeight > screenHeight) sliceHeight = screenHeight;

        SDL_FRect wallSlice = {
            static_cast<float>(x),
            (screenHeight - sliceHeight) / 2.0f + viewOffsetY,
            1.0f,
            sliceHeight
        };

        // Clamp tile bounds
        int tileX = static_cast<int>(playerX + rayX * dist);
        int tileY = static_cast<int>(playerY + rayY * dist);
        tileX = std::clamp(tileX, 0, mapWidth - 1);
        tileY = std::clamp(tileY, 0, mapHeight - 1);

        // Slightly reduced light brightness
        int light = lightMap[tileY][tileX];
        float lightFactor = static_cast<float>(light) / 2.0f * 0.75f; // dimmed to 75% of normal

        // Fog
        const float fogStart = 1.0f;
        const float fogEnd = 5.0f;
        float fogFactor = 1.0f - (dist - fogStart) / (fogEnd - fogStart);
        fogFactor = std::clamp(fogFactor, 0.0f, 1.0f);

        float finalFactor = std::clamp(lightFactor * fogFactor, 0.0f, 1.0f);

        // Color output
        int baseR = 180, baseG = 180, baseB = 180;
        int r = static_cast<int>(baseR * finalFactor);
        int g = static_cast<int>(baseG * finalFactor);
        int b = static_cast<int>(baseB * finalFactor);

        SDL_SetRenderDrawColor(renderer, r, g, b, 255);
        SDL_RenderFillRect(renderer, &wallSlice);
    }
}

void GameScene::renderPickups(SDL_Renderer* renderer) {
    const float screenWidth = 800.0f;
    const float screenHeight = 600.0f;
    float time = SDL_GetTicks() / 1000.0f;

    nearPickup = false;

    for (const auto& p : pickups) {
        if (p.collected) continue;

        float dx = p.x - playerX;
        float dy = p.y - playerY;
        float dist = sqrtf(dx * dx + dy * dy);
        if (dist <= 0.01f) continue;  // avoid div by zero

        float angleToPickup = atan2f(dy, dx);
        float relativeAngle = angleToPickup - playerAngle;

        if (relativeAngle < -M_PI) relativeAngle += 2.0f * M_PI;
        if (relativeAngle > M_PI)  relativeAngle -= 2.0f * M_PI;

        if (fabsf(relativeAngle) > FOV / 2.0f) continue;  // not in view

        float sliceX = (relativeAngle + FOV / 2.0f) / FOV * screenWidth;
        float scale = 1.0f / dist * 300.0f;
        float yOffset = sinf(time * 2.0f) * 10.0f;

        SDL_FPoint tri[3] = {
            { sliceX, (screenHeight / 2.0f) - scale + yOffset },
            { sliceX - scale / 2.0f, (screenHeight / 2.0f) + scale + yOffset },
            { sliceX + scale / 2.0f, (screenHeight / 2.0f) + scale + yOffset }
        };

        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
        SDL_RenderLines(renderer, tri, 3);
        SDL_RenderLine(renderer, tri[2].x, tri[2].y, tri[0].x, tri[0].y);

        float distSq = dx * dx + dy * dy;

        if (distSq < 1.0f) {
            nearPickup = true;
            break;
        }
    }
}

void GameScene::render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 15, 15, 15, 255);
    SDL_RenderClear(renderer);

    renderWallsRaycast(renderer);
	drawMap(renderer);
    drawPlayer(renderer);
	renderPickups(renderer);

    char buffer[64];
    snprintf(buffer, sizeof(buffer), "Pickups Collected: %d", pickupsCollected);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDebugText(renderer, 10.0f, 10.0f, buffer);

    if (nearPickup) {
        SDL_RenderDebugText(renderer, 10.0f, 30.0f, "Press 'E' to pick up");
    }

    SDL_RenderPresent(renderer);
}

void GameScene::drawMap(SDL_Renderer* renderer) {
    const int tileSize = 32;

    for (int y = 0; y < mapHeight; ++y) {
        for (int x = 0; x < mapWidth; ++x) {
            if (map[y][x] == 1) {
                SDL_FRect rect = {
                    static_cast<float>(x * tileSize),
                    static_cast<float>(y * tileSize),
                    static_cast<float>(tileSize),
                    static_cast<float>(tileSize)
                };
                SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }
}

void GameScene::drawPlayer(SDL_Renderer* renderer) {
    const int tileSize = 32;

    SDL_FRect rect = {
        playerX * tileSize - 4,
        playerY * tileSize - 4,
        8,
        8
    };
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &rect);
}



