#pragma once
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <string>
#include <unordered_map>
#include "Engine/Graphics/Renderer.hpp"

namespace Graphics {
    class TextureManager {
    public:
        TextureManager();
        ~TextureManager();

        void init(Renderer* renderer);
        void cleanup();

        SDL_Texture* loadTexture(const std::string& id, const std::string& path);
        
        void draw(const std::string& id, float x, float y, float width, float height);
        void drawFrame(const std::string& id, float x, float y, float width, float height, int row, int frame);

    private:
        Renderer* m_renderer;
        std::unordered_map<std::string, SDL_Texture*> m_textures;
    };
}
