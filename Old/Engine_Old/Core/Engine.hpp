#pragma once
#include "Engine/Core/Window.hpp"
#include "Engine/Core/InputManager.hpp"
#include "Engine/Core/Time.hpp"
#include "Engine/Graphics/Renderer.hpp"
#include "Engine/Graphics/TextureManager.hpp"
#include "Engine/Audio/SoundManager.hpp"
#include "Engine/Game/IGame.hpp"
#include <memory>

namespace Core {
    class Engine {
    public:
        Engine(const std::string& title, int width, int height);
        ~Engine();

        bool init(std::unique_ptr<Game::IGame> game);
        void run();

    private:
        void handleEvents();
        
        bool m_isRunning;
        Window m_window;
        InputManager m_input;
        Time m_time;
        
        Graphics::Renderer m_renderer;
        Graphics::TextureManager m_textures;
        Audio::SoundManager m_audio;

        std::unique_ptr<Game::IGame> m_game;
    };
}
