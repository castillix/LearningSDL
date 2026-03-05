#include "Engine/Audio/SoundManager.hpp"
#include <SDL3/SDL.h>

namespace Audio {
    SoundManager::SoundManager() {}
    SoundManager::~SoundManager() { cleanup(); }

    bool SoundManager::init() {
        SDL_Log("SoundManager (Stubbed) Initialized!");
        return true;
    }

    void SoundManager::cleanup() {
        m_sounds.clear();
        m_music.clear();
    }

    void* SoundManager::loadSound(const std::string& id, const std::string& path) { return nullptr; }
    void* SoundManager::loadMusic(const std::string& id, const std::string& path) { return nullptr; }
    void SoundManager::playSound(const std::string& id, int loops) {}
    void SoundManager::playMusic(const std::string& id, int loops) {}
    void SoundManager::stopMusic() {}
}
