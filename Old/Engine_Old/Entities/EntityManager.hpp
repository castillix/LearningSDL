#pragma once
#include "Engine/Entities/Entity.hpp"
#include <vector>
#include <memory>

namespace Entities {
    class EntityManager {
    public:
        void update(float deltaTime, const Core::InputManager& input);
        void render(Graphics::Renderer* renderer);

        void addEntity(std::unique_ptr<Entity> entity);
        void clear();

    private:
        std::vector<std::unique_ptr<Entity>> m_entities;
    };
}
