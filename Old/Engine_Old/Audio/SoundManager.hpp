#pragma once
#include <string>
#include <unordered_map>

namespace Audio {
    class SoundManager {
    public:
        SoundManager();
        ~SoundManager();

        bool init();
        void cleanup();

        void* loadSound(const std::string& id, const std::string& path);
        void* loadMusic(const std::string& id, const std::string& path);

        void playSound(const std::string& id, int loops = 0);
        void playMusic(const std::string& id, int loops = -1);
        void stopMusic();

    private:
        std::unordered_map<std::string, void*> m_sounds;
        std::unordered_map<std::string, void*> m_music;
    };
}
