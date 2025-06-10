#include "TextureManager.h"
#include <SDL3_Image/SDL_image.h>

bool TextureManager::loadTexture(SDL_Renderer* renderer, const std::string& id, const std::string& filePath) {
    SDL_Log("Loading texture: %s", filePath.c_str());
    SDL_Surface* surface = SDL_LoadBMP(filePath.c_str());  // Use SDL_image for PNG later
    if (!surface) {
        SDL_Log("Failed to load image %s: %s", filePath.c_str(), SDL_GetError());
        return false;
    }

  

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_DestroySurface(surface);

    if (!texture) {
        SDL_Log("Failed to create texture: %s", SDL_GetError());
        return false;
    }
    if (texture) {
        SDL_Log("Texture loaded OK: %s", id.c_str());
    }
    

    textures[id] = texture;
    return true;
}

SDL_Texture* TextureManager::getTexture(const std::string& id) const {
    auto it = textures.find(id);
    return (it != textures.end()) ? it->second : nullptr;
}

void TextureManager::unloadAll() {
    for (auto& pair : textures) {
        SDL_DestroyTexture(pair.second);
    }
    textures.clear();
}
