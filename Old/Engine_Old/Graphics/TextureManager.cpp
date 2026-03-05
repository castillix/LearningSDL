#include "Engine/Graphics/TextureManager.hpp"

namespace Graphics {
    TextureManager::TextureManager() : m_renderer(nullptr) {}

    TextureManager::~TextureManager() {
        cleanup();
    }

    void TextureManager::init(Renderer* renderer) {
        m_renderer = renderer;
    }

    void TextureManager::cleanup() {
        for (auto& pair : m_textures) {
            SDL_DestroyTexture(pair.second);
        }
        m_textures.clear();
    }

    SDL_Texture* TextureManager::loadTexture(const std::string& id, const std::string& path) {
        if (m_textures.find(id) != m_textures.end()) {
            return m_textures[id];
        }

        if (!m_renderer || !m_renderer->getSDLRenderer()) {
            return nullptr;
        }

        SDL_Texture* texture = IMG_LoadTexture(m_renderer->getSDLRenderer(), path.c_str());
        if (!texture) {
            SDL_Log("Failed to load texture %s! SDL_image Error: %s\n", path.c_str(), SDL_GetError());
            return nullptr;
        }

        m_textures[id] = texture;
        return texture;
    }

    void TextureManager::draw(const std::string& id, float x, float y, float width, float height) {
        if (!m_renderer) return;

        auto it = m_textures.find(id);
        if (it != m_textures.end()) {
            SDL_FRect destRect = { x, y, width, height };
            SDL_RenderTexture(m_renderer->getSDLRenderer(), it->second, nullptr, &destRect);
        }
    }

    void TextureManager::drawFrame(const std::string& id, float x, float y, float width, float height, int row, int frame) {
        if (!m_renderer) return;

        auto it = m_textures.find(id);
        if (it != m_textures.end()) {
            SDL_FRect srcRect = { static_cast<float>(frame * width), static_cast<float>(row * height), width, height };
            SDL_FRect destRect = { x, y, width, height };
            SDL_RenderTexture(m_renderer->getSDLRenderer(), it->second, &srcRect, &destRect);
        }
    }
}
