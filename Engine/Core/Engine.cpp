#include "Engine/Core/Engine.hpp"
#include <SDL3/SDL.h>
#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_opengl3.h>

namespace Engine {
namespace Core {

    MainEngine::MainEngine(std::unique_ptr<Interfaces::IWindow> window,
                           std::unique_ptr<Interfaces::IRenderer> renderer,
                           std::unique_ptr<Interfaces::IInput> input)
        : m_window(std::move(window)), m_renderer(std::move(renderer)), m_input(std::move(input)) {}

    MainEngine::~MainEngine() {
        while (!m_states.empty()) {
            popState();
        }

        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplSDL3_Shutdown();
        ImGui::DestroyContext();

        if (m_renderer) m_renderer->close();
        if (m_window) m_window->close();
    }

    bool MainEngine::init(const std::string& title, int width, int height) {
        if (!m_window->init(title, width, height)) return false;
        if (!m_renderer->init(m_window->getNativeWindow())) return false;
        
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        ImGui::StyleColorsDark();

        ImGui_ImplSDL3_InitForOpenGL((SDL_Window*)m_window->getNativeWindow(), nullptr);
        ImGui_ImplOpenGL3_Init("#version 330 core");

        m_lastTime = SDL_GetPerformanceCounter();
        m_isRunning = true;
        return true;
    }

    void MainEngine::pushState(std::unique_ptr<Interfaces::IGameState> state) {
        m_states.push_back(std::move(state));
        m_states.back()->init(this);
    }

    void MainEngine::popState() {
        if (!m_states.empty()) {
            m_states.back()->cleanup();
            m_states.pop_back();
        }
    }

    void MainEngine::run() {
        while (m_isRunning) {
            uint64_t currentTime = SDL_GetPerformanceCounter();
            float deltaTime = (currentTime - m_lastTime) / (float)SDL_GetPerformanceFrequency();
            m_lastTime = currentTime;
            
            // Clamp huge spikes (e.g. from moving window)
            if (deltaTime > 0.1f) deltaTime = 0.1f;

            m_input->updateBegin();
            m_input->pollEvents(this);

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplSDL3_NewFrame();
            ImGui::NewFrame();

            if (!m_states.empty()) {
                m_states.back()->update(deltaTime);
                
                // State might have called quit() during update
                if (!m_isRunning) break;

                m_renderer->clear(0.1f, 0.1f, 0.1f, 1.0f);
                m_states.back()->render(m_renderer.get());
                
                m_states.back()->renderUI();
                ImGui::Render();
                ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

                m_window->swapBuffers();
            } else {
                m_isRunning = false; // Nowhere to go
            }
        }
    }

}
}
