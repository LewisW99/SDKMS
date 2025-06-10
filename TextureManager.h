#pragma once
#include <SDL3/SDL.h>
#include <string>
#include <unordered_map>

class TextureManager {
public:
    bool loadTexture(SDL_Renderer* renderer, const std::string& id, const std::string& filePath);
    SDL_Texture* getTexture(const std::string& id) const;
    void unloadAll();

private:
    std::unordered_map<std::string, SDL_Texture*> textures;
};
