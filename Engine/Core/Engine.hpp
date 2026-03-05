#pragma once
#include "Engine/Interfaces/IEngine.hpp"
#include "Engine/Interfaces/IWindow.hpp"
#include "Engine/Interfaces/IRenderer.hpp"
#include "Engine/Interfaces/IInput.hpp"
#include "Engine/Interfaces/IGameState.hpp"
#include <memory>
#include <vector>
#include <cstdint>

namespace Engine {
namespace Core {

    class MainEngine : public Interfaces::IEngine {
    public:
        MainEngine(std::unique_ptr<Interfaces::IWindow> window,
                   std::unique_ptr<Interfaces::IRenderer> renderer,
                   std::unique_ptr<Interfaces::IInput> input);
        ~MainEngine() override;

        bool init(const std::string& title, int width, int height);
        void pushState(std::unique_ptr<Interfaces::IGameState> state);
        void popState();
        void run();

        void quit() override { m_isRunning = false; }
        Interfaces::IWindow* getWindow() override { return m_window.get(); }
        Interfaces::IRenderer* getRenderer() override { return m_renderer.get(); }
        Interfaces::IInput* getInput() override { return m_input.get(); }

    private:
        std::unique_ptr<Interfaces::IWindow> m_window;
        std::unique_ptr<Interfaces::IRenderer> m_renderer;
        std::unique_ptr<Interfaces::IInput> m_input;

        std::vector<std::unique_ptr<Interfaces::IGameState>> m_states;

        bool m_isRunning = false;
        uint64_t m_lastTime = 0;
    };

}
}
