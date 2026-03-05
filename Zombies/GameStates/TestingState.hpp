#pragma once
#include "Engine/Interfaces/IGameState.hpp"
#include "Engine/Graphics/VoxelMap.hpp"
#include "Engine/Entities/Player.hpp"
#include "Engine/Entities/Zombie.hpp"
#include "Engine/Entities/Weapon.hpp"
#include "Engine/AI/FlowField.hpp"
#include "Engine/Physics/Raycaster.hpp"
#include "Engine/Graphics/Shader.hpp"
#include <memory>
#include <vector>

class TestingState : public Engine::Interfaces::IGameState {
public:
    TestingState() = default;
    ~TestingState() override = default;

    void init(Engine::Interfaces::IEngine* engine) override;
    void update(float deltaTime) override;
    void render(Engine::Interfaces::IRenderer* renderer) override;
    void renderUI() override;
    void cleanup() override;

private:
    Engine::Interfaces::IEngine* m_engine = nullptr;
    std::unique_ptr<Engine::Graphics::VoxelMap> m_map;
    std::unique_ptr<Engine::Entities::Player> m_player;
    Engine::Graphics::Shader m_shader;
    
    std::vector<std::unique_ptr<Engine::Entities::Zombie>> m_zombies;
    std::unique_ptr<Engine::AI::FlowField> m_flowField;
    std::unique_ptr<Engine::Physics::VoxelRaycaster> m_raycaster;
    Engine::Entities::Weapon m_weapon;

    int m_score = 500;
    bool m_doorBought = false;
};
