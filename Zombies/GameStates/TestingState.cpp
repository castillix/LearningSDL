#include "TestingState.hpp"
#include "Engine/Interfaces/IEngine.hpp"
#include "Engine/Interfaces/IWindow.hpp"
#include "Engine/Interfaces/IInput.hpp"
#include <imgui.h>
#include <SDL3/SDL.h>

void TestingState::init(Engine::Interfaces::IEngine* engine) {
    m_engine = engine;
    m_engine->getWindow()->setRelativeMouseMode(true);

    m_shader.compile("Zombies/assets/shaders/basic_vertex.glsl", "Zombies/assets/shaders/basic_fragment.glsl");

    m_map = std::make_unique<Engine::Graphics::VoxelMap>();
    
    // Generate simple flat floor and a ceiling
    for(int x = 0; x < 64; ++x) {
        for(int z = 0; z < 64; ++z) {
            m_map->setBlock(x, 0, z, 1); // Floor
            m_map->setBlock(x, 5, z, 1); // Ceiling
            
            if (x == 0 || x == 63 || z == 0 || z == 63) {
                // Outer Walls
                m_map->setBlock(x, 1, z, 2); 
                m_map->setBlock(x, 2, z, 2); 
                m_map->setBlock(x, 3, z, 2); 
                m_map->setBlock(x, 4, z, 2); 
            }
        }
    }

    // Dividing wall at x=32
    for(int z = 1; z < 63; ++z) {
        for(int y = 1; y < 5; ++y) {
            if (z >= 30 && z <= 33 && y <= 3) {
                // Set the specific DOOR block which is solid until opened
                m_map->setBlock(32, y, z, 4); 
            } else {
                m_map->setBlock(32, y, z, 2); // Solid wall
            }
        }
    }

    // Room 1 barricade wall at z=16
    for(int x = 1; x < 32; ++x) {
        for (int y = 1; y < 5; ++y) {
            if (x >= 14 && x <= 16 && (y == 2 || y == 3)) {
                continue; // Window hole (climbs over y=1)
            }
            m_map->setBlock(x, y, 16, 2);
        }
    }

    // Room 2 barricade wall at z=48
    for(int x = 33; x < 63; ++x) {
        for (int y = 1; y < 5; ++y) {
            if (x >= 46 && x <= 48 && (y == 2 || y == 3)) {
                continue; // Window hole
            }
            m_map->setBlock(x, y, 48, 2);
        }
    }

    m_player = std::make_unique<Engine::Entities::Player>(glm::vec3(16.0f, 1.0f, 32.0f));
    
    m_flowField = std::make_unique<Engine::AI::FlowField>();
    m_raycaster = std::make_unique<Engine::Physics::VoxelRaycaster>();
    
    // Initial zombies in Room 1 spawn zone (z < 16)
    m_zombies.push_back(std::make_unique<Engine::Entities::Zombie>(glm::vec3(15.0f, 2.0f, 10.0f)));
    m_zombies.push_back(std::make_unique<Engine::Entities::Zombie>(glm::vec3(5.0f, 2.0f, 5.0f)));
}

void TestingState::update(float deltaTime) {
    // Escape handled by engine loop automatically
    m_player->handleInput(m_engine->getInput());
    m_player->update(deltaTime, m_map.get());

    if (m_engine->getInput()->isMouseJustPressed(1)) {
        m_weapon.fire(m_raycaster.get(), m_map.get(), m_player->getCamera().getPosition(), m_player->getCamera().getFront(), m_zombies);
    }

    glm::vec3 playerPos = m_player->getPosition();
    if (!m_doorBought && m_engine->getInput()->isKeyJustPressed(SDLK_E)) {
        if (glm::distance(playerPos, glm::vec3(32.0f, 1.0f, 31.5f)) < 5.0f) {
            if (m_score >= 500) {
                m_score -= 500;
                m_doorBought = true;
                // Remove door blocks
                for(int z = 30; z <= 33; ++z) {
                    for(int y = 1; y <= 3; ++y) {
                        m_map->setBlock(32, y, z, 0);
                    }
                }
                
                // Spawn Room 2 zombies
                m_zombies.push_back(std::make_unique<Engine::Entities::Zombie>(glm::vec3(47.0f, 2.0f, 55.0f)));
                m_zombies.push_back(std::make_unique<Engine::Entities::Zombie>(glm::vec3(55.0f, 2.0f, 55.0f)));
            }
        }
    }

    m_flowField->setTarget(glm::ivec3(m_player->getPosition()));
    m_flowField->updateRecalculate(m_map.get());

    for (auto& z : m_zombies) {
        z->navigate(m_flowField.get(), deltaTime);
        z->update(deltaTime, m_map.get());
    }
}

void TestingState::render(Engine::Interfaces::IRenderer* renderer) {
    m_shader.use();

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
    glm::mat4 view = m_player->getCamera().getViewMatrix();

    m_shader.setMat4("projection", projection);
    m_shader.setMat4("view", view);
    m_shader.setMat4("model", glm::mat4(1.0f));

    m_map->render(renderer);
    m_player->render(renderer);

    for (auto& z : m_zombies) {
        glm::mat4 model = glm::translate(glm::mat4(1.0f), z->getPosition());
        // For simple placeholder, we'll just color them red/green in texture, or if shader allows, set uniform.
        // We'll just draw them at their pos.
        m_shader.setMat4("model", model);
        z->render(renderer);
    }
}

void TestingState::renderUI() {
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
    if (ImGui::Begin("Debug Overlay", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav)) {
        ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
        ImGui::Text("Frame Time: %.3f ms", 1000.0f / ImGui::GetIO().Framerate);
        
        glm::vec3 pos = m_player->getPosition();
        ImGui::Separator();
        ImGui::Text("Player POS: [%.1f, %.1f, %.1f]", pos.x, pos.y, pos.z);
        
        ImGui::Separator();
        ImGui::Text("Score: %d", m_score);
        if (!m_doorBought) {
            ImGui::Text("Door to Room 2 locked (Cost: 500)");
        } else {
            ImGui::Text("Door Unlocked!");
        }
    }
    ImGui::End();
}

void TestingState::cleanup() {
    // Let smart pointers handle cleanup
}
