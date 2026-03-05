#pragma once
#include <SDL3/SDL.h>
#include <vector>
#include "Engine/Graphics/Renderer.hpp"

namespace Game {
    class Terrain {
    public:
        Terrain(int width, int height);
        ~Terrain();

        void init(Graphics::Renderer* renderer);
        void updateTexture();
        void render(Graphics::Renderer* renderer);

        // Core Destructibility
        void destroyCircle(int cx, int cy, int radius);

        // Collision Query
        bool isSolid(int x, int y) const;

        int getWidth() const { return m_width; }
        int getHeight() const { return m_height; }

    private:
        void generatePerlinLike();
        void setPixelFast(int x, int y, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

        int m_width;
        int m_height;

        // RGBA raw pixel data
        std::vector<Uint8> m_pixels;
        SDL_Texture* m_texture;
        Graphics::Renderer* m_renderer;

        // Dirty flag to avoid uploading texture every frame if no terrain was destroyed
        bool m_isDirty; 
    };
}
