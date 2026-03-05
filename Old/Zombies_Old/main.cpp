#include <glad/glad.h>
#include "Engine/Core/Engine.hpp"
#include "Engine/Graphics/Shader.hpp"
#include "Engine/Graphics/Camera3D.hpp"
#include "Engine/Graphics/Chunk.hpp"
#include "Engine/Entities/Player.hpp"
#include "Engine/Entities/Zombie.hpp"
#include "Engine/Physics/Raycaster.hpp"
#include "Engine/Game/VoxelMap.hpp"
#include "Engine/Game/RoundManager.hpp"
#include "Engine/AI/FlowField.hpp"
#include "Engine/Entities/MysteryBox.hpp"
#include "Engine/Entities/Barrier.hpp"
#include "Engine/Entities/PerkMachine.hpp"
#include <SDL3/SDL.h>
#include <memory>
#include <vector>

class ZombiesGame : public Game::IGame {
private:
    enum class GameState { MENU, PLAYING, GAMEOVER };
    GameState m_state = GameState::MENU;

    Graphics::Shader m_shader;
    std::unique_ptr<Entities::Player> m_player;
    std::vector<std::unique_ptr<Entities::Zombie>> m_zombies;
    std::unique_ptr<Game::VoxelMap> m_map;
    std::unique_ptr<AI::FlowField> m_flowField;
    std::unique_ptr<Game::RoundManager> m_round;
    std::unique_ptr<Entities::MysteryBox> m_mysteryBox;
    std::vector<std::unique_ptr<Entities::Barrier>> m_barriers;
    std::vector<std::unique_ptr<Entities::PerkMachine>> m_perkMachines;

    float m_pathTimer = 0.0f;
public:
    ZombiesGame() = default;
    ~ZombiesGame() override = default;

    bool init(Graphics::Renderer* renderer, Graphics::TextureManager* textures, Audio::SoundManager* audio) override {
        SDL_Log("ZombiesGame Initialized!");
        
        if (!m_shader.compile("Zombies/assets/shaders/basic_vertex.glsl", "Zombies/assets/shaders/basic_fragment.glsl")) {
            SDL_Log("Failed to compile shaders!");
            return false;
        }

        m_map = std::make_unique<Game::VoxelMap>();
        
        // Try load
        if (!m_map->loadFromFile("Zombies/assets/maps/test_map.dat")) {
            SDL_Log("No map found, generating default layout...");
            // Generate a 2x2 chunk map (64x64 voxels)
            for(int x = 0; x < 64; ++x) {
                for(int z = 0; z < 64; ++z) {
                    m_map->setBlock(x, 0, z, 1); // Floor
                    
                    if (x == 0 || x == 63 || z == 0 || z == 63) {
                        m_map->setBlock(x, 1, z, 2); // Walls
                        m_map->setBlock(x, 2, z, 2); 
                    }
                }
            }
            // Add a barrier zone
            m_map->setBlock(32, 1, 32, 3);
            m_map->setBlock(32, 2, 32, 3);
            m_map->saveToFile("Zombies/assets/maps/test_map.dat");
        }

        m_player = std::make_unique<Entities::Player>(glm::vec3(32.0f, 4.0f, 32.0f));

        m_round = std::make_unique<Game::RoundManager>();
        m_flowField = std::make_unique<AI::FlowField>(64, 16, 64); // 64x16x64 pathfinding volume
        m_mysteryBox = std::make_unique<Entities::MysteryBox>(glm::vec3(35.0f, 1.0f, 35.0f));

        m_barriers.clear();
        m_perkMachines.clear();
        // Place a barrier near the center area
        m_barriers.push_back(std::make_unique<Entities::Barrier>(glm::vec3(30.0f, 1.0f, 35.0f), 100));

        // Place a Juggernog machine
        m_perkMachines.push_back(std::make_unique<Entities::PerkMachine>(glm::vec3(20.0f, 1.0f, 20.0f), Entities::PerkType::JUGGERNOG, 2500));
        
        SDL_Log("--- MAIN MENU ---");
        SDL_Log("Press SPACE to Start!");

        return true;
    }

    void update(float deltaTime, const Core::InputManager& input) override {
        // Clamp deltaTime to prevent physics tunneling
        if (deltaTime > 0.05f) deltaTime = 0.05f;

        if (input.isKeyJustPressed(SDL_SCANCODE_ESCAPE)) {
            // Quick exit mapping can be added if needed, but Engine handles Window closing
        }

        if (m_state == GameState::MENU) {
            if (input.isKeyJustPressed(SDL_SCANCODE_SPACE)) {
                m_state = GameState::PLAYING;
                SDL_Log("Game Started!");
                SDL_Window* win = SDL_GetKeyboardFocus();
                if (win) SDL_SetWindowRelativeMouseMode(win, true);
            }
            return;
        } else if (m_state == GameState::GAMEOVER) {
            if (input.isKeyJustPressed(SDL_SCANCODE_SPACE)) {
                // Re-initialize the game state for a restart
                init(nullptr, nullptr, nullptr);
                m_state = GameState::PLAYING;
                SDL_Log("Game Restarted!");
                SDL_Window* win = SDL_GetKeyboardFocus();
                if (win) SDL_SetWindowRelativeMouseMode(win, true);
            }
            return;
        }

        // --- PLAYING STATE ---
        if (m_player->isDead()) {
            m_state = GameState::GAMEOVER;
            SDL_Log("--- GAME OVER ---");
            SDL_Log("You survived %d rounds.", m_round->getCurrentRound());
            SDL_Log("Press SPACE to Restart.");
            SDL_Window* win = SDL_GetKeyboardFocus();
            if (win) SDL_SetWindowRelativeMouseMode(win, false);
            return;
        }

        // Player controls
        m_player->moveForward = input.isKeyHeld(SDL_SCANCODE_W);
        m_player->moveBackward = input.isKeyHeld(SDL_SCANCODE_S);
        m_player->moveLeft = input.isKeyHeld(SDL_SCANCODE_A);
        m_player->moveRight = input.isKeyHeld(SDL_SCANCODE_D);
        m_player->jump = input.isKeyHeld(SDL_SCANCODE_SPACE);

        // Process mouse movement explicitly
        float dx, dy;
        input.getMouseDelta(dx, dy);
        m_player->processMouse(dx, -dy); // Invert Y for standard FPS look

        m_player->update(deltaTime, m_map.get());

        // Generate Pathfinding Field ~1 times a second
        m_pathTimer += deltaTime;
        if (m_pathTimer > 1.0f) {
            m_pathTimer = 0.0f;
            m_flowField->generate(m_map.get(), glm::ivec3(m_player->getPosition()));
        }

        m_round->update(deltaTime, m_zombies, m_map.get(), m_player.get());

        m_mysteryBox->update(deltaTime);
        for (auto& b : m_barriers) {
            b->update(deltaTime);
        }

        for (auto& z : m_zombies) {
            z->update(deltaTime, m_map.get(), m_flowField.get(), m_barriers);
        }

        // Left click to fire
        if (input.isMouseJustPressed(1)) {
            if (Entities::Weapon* w = m_player->getActiveWeapon()) {
                w->fire(m_player->getCamera().getPosition(), m_player->getCamera().getFront(), m_map.get(), m_zombies);
            }
        }
        
        // R to reload
        if (input.isKeyJustPressed(SDL_SCANCODE_R)) {
            if (Entities::Weapon* w = m_player->getActiveWeapon()) {
                w->reload();
            }
        }

        // F to interact
        if (input.isKeyJustPressed(SDL_SCANCODE_F)) {
            // Check Raycast against MysteryBox
            float tHit;
            glm::vec3 origin = m_player->getCamera().getPosition();
            glm::vec3 dir = m_player->getCamera().getFront();
            if (Entities::Weapon* w = m_player->getActiveWeapon()) { // Use weapon's AABB checker
                // A bit hacky, normally static physics wrapper. We'll use manual AABB check for now
                glm::vec3 invDir = 1.0f / dir;
                Physics::AABB box = m_mysteryBox->getAABB();
                glm::vec3 t0 = (box.minExtents - origin) * invDir;
                glm::vec3 t1 = (box.maxExtents - origin) * invDir;
                glm::vec3 tmin = glm::min(t0, t1);
                glm::vec3 tmax = glm::max(t0, t1);
                float tNear = glm::max(glm::max(tmin.x, tmin.y), tmin.z);
                float tFar = glm::min(glm::min(tmax.x, tmax.y), tmax.z);

                if (tNear <= tFar && tFar >= 0.0f && tNear < 3.0f) {
                    m_mysteryBox->interact(m_player.get());
                    SDL_Log("Mystery Box Spin triggered!");
                }

                // Check Barriers
                for (auto& b : m_barriers) {
                    Physics::AABB box = b->getAABB();
                    t0 = (box.minExtents - origin) * invDir;
                    t1 = (box.maxExtents - origin) * invDir;
                    tmin = glm::min(t0, t1);
                    tmax = glm::max(t0, t1);
                    tNear = glm::max(glm::max(tmin.x, tmin.y), tmin.z);
                    tFar = glm::min(glm::min(tmax.x, tmax.y), tmax.z);

                    if (tNear <= tFar && tFar >= 0.0f && tNear < 3.0f) {
                        if (b->repair()) {
                            m_player->addPoints(10);
                        }
                    }
                }

                // Check Perk Machines
                for (auto& p : m_perkMachines) {
                    Physics::AABB box = p->getAABB();
                    t0 = (box.minExtents - origin) * invDir;
                    t1 = (box.maxExtents - origin) * invDir;
                    tmin = glm::min(t0, t1);
                    tmax = glm::max(t0, t1);
                    tNear = glm::max(glm::max(tmin.x, tmin.y), tmin.z);
                    tFar = glm::min(glm::min(tmax.x, tmax.y), tmax.z);

                    if (tNear <= tFar && tFar >= 0.0f && tNear < 3.0f) {
                        p->interact(m_player.get());
                    }
                }
            }
        }
    }

    void render(Graphics::Renderer* renderer) override {
        // Clear background to a dark color for Zombies using OpenGL directly
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_shader.use();

        // Pass matrices
        // Basic View/Proj update for menu as well
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
        glm::mat4 view = m_player->getCamera().getViewMatrix();

        m_shader.use();
        m_shader.setMat4("projection", projection);
        m_shader.setMat4("view", view);

        if (m_state == GameState::MENU) {
            glClearColor(0.1f, 0.1f, 0.3f, 1.0f); // Blue menu background
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            return;
        }

        // Set model matrix to identity for chunks
        m_shader.setMat4("model", glm::mat4(1.0f));

        // Draw chunks
        if (m_map) m_map->render();

        m_mysteryBox->render(m_shader);
        for (auto& b : m_barriers) {
            b->render(m_shader);
        }
        for (auto& p : m_perkMachines) {
            p->render(m_shader);
        }

        // Draw zombies
        for (auto& z : m_zombies) {
            z->render(m_shader);
        }

        if (m_state == GameState::GAMEOVER) {
            // Optional: Draw a red quad over the screen using a different shader 
            // For now, the game rests showing the last frame with zombies
        }
    }

    void cleanup() override {
        SDL_Log("ZombiesGame Cleaned up!");
    }
};

int main(int argc, char* argv[]) {
    Core::Engine engine("Zombies", 800, 600);
    auto game = std::make_unique<ZombiesGame>();

    if (engine.init(std::move(game))) {
        engine.run();
    }

    return 0;
}
