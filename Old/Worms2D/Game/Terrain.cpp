#include "Terrain.hpp"
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <cstdlib>

namespace Game {
    Terrain::Terrain(int width, int height) : m_width(width), m_height(height), m_texture(nullptr), m_renderer(nullptr), m_isDirty(true) {
        // 4 bytes per pixel (RGBA)
        m_pixels.resize(width * height * 4, 0); 
    }

    Terrain::~Terrain() {
        if (m_texture) {
            SDL_DestroyTexture(m_texture);
        }
    }

    void Terrain::init(Graphics::Renderer* renderer) {
        m_renderer = renderer;
        
        m_texture = SDL_CreateTexture(renderer->getSDLRenderer(), 
                                      SDL_PIXELFORMAT_RGBA32, 
                                      SDL_TEXTUREACCESS_STREAMING, 
                                      m_width, m_height);
                                      
        if (!m_texture) {
            SDL_Log("Terrain: Failed to create texture: %s", SDL_GetError());
        }

        SDL_SetTextureBlendMode(m_texture, SDL_BLENDMODE_BLEND);

        generatePerlinLike();
        updateTexture();
    }

    void Terrain::generatePerlinLike() {
        // Simple random hills using sine waves for placeholder
        for (int x = 0; x < m_width; ++x) {
            
            // Base height + sum of sines
            float height = 300.0f 
                         + std::sin(x * 0.01f) * 50.0f
                         + std::sin(x * 0.05f) * 20.0f;
                         
            int surfaceY = static_cast<int>(height);

            for (int y = 0; y < m_height; ++y) {
                if (y > surfaceY) {
                    // Solid ground (Greenish dirt)
                    if (y - surfaceY < 10) {
                        setPixelFast(x, y, 34, 139, 34, 255); // Grass
                    } else {
                        setPixelFast(x, y, 139, 69, 19, 255); // Dirt
                    }
                } else {
                    // Empty sky (Transparent so background shows)
                    setPixelFast(x, y, 0, 0, 0, 0); 
                }
            }
        }
        m_isDirty = true;
    }

    void Terrain::setPixelFast(int x, int y, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
        if (x < 0 || x >= m_width || y < 0 || y >= m_height) return;
        int index = (y * m_width + x) * 4;
        
        // RGBA depending on native endianness, SDL_PIXELFORMAT_RGBA32 treats bytes in memory strictly as R,G,B,A on most architectures. 
        // We will assume standard RGBA byte order:
        m_pixels[index + 0] = r;
        m_pixels[index + 1] = g;
        m_pixels[index + 2] = b;
        m_pixels[index + 3] = a;
    }

    void Terrain::destroyCircle(int cx, int cy, int radius) {
        bool changed = false;
        int r2 = radius * radius;

        int minX = std::max(0, cx - radius);
        int maxX = std::min(m_width - 1, cx + radius);
        int minY = std::max(0, cy - radius);
        int maxY = std::min(m_height - 1, cy + radius);

        for (int y = minY; y <= maxY; ++y) {
            for (int x = minX; x <= maxX; ++x) {
                int dx = x - cx;
                int dy = y - cy;
                if (dx * dx + dy * dy <= r2) {
                    setPixelFast(x, y, 0, 0, 0, 0); // Erase pixel
                    changed = true;
                }
            }
        }

        if (changed) {
            m_isDirty = true;
        }
    }

    bool Terrain::isSolid(int x, int y) const {
        if (x < 0 || x >= m_width || y < 0 || y >= m_height) {
            return false; // Bounds are empty air
        }
        
        int index = (y * m_width + x) * 4;
        // Check alpha channel
        return m_pixels[index + 3] > 0;
    }

    void Terrain::updateTexture() {
        if (!m_isDirty || !m_texture) return;

        // Using UpdateTexture is universally safer than LockTexture across all backends for this use case
        int mPitch = m_width * 4;
        if (!SDL_UpdateTexture(m_texture, nullptr, m_pixels.data(), mPitch)) {
            SDL_Log("Could not update terrain texture! %s", SDL_GetError());
        }

        m_isDirty = false;
    }

    void Terrain::render(Graphics::Renderer* renderer) {
        if (m_isDirty) {
            updateTexture();
        }

        if (m_texture && renderer && renderer->getSDLRenderer()) {
            SDL_FRect destRect = { 0.0f, 0.0f, static_cast<float>(m_width), static_cast<float>(m_height) };
            SDL_RenderTexture(renderer->getSDLRenderer(), m_texture, nullptr, &destRect);
        }
    }
}
