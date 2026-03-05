#include "Engine/Entities/EntityManager.hpp"
#include <algorithm>

namespace Entities {
    void EntityManager::update(float deltaTime, const Core::InputManager& input) {
        for (auto& entity : m_entities) {
            entity->update(deltaTime, input);
        }

        m_entities.erase(
            std::remove_if(m_entities.begin(), m_entities.end(),
                [](const std::unique_ptr<Entity>& e) { return e->isDestroyed(); }),
            m_entities.end());
    }

    void EntityManager::render(Graphics::Renderer* renderer) {
        for (auto& entity : m_entities) {
            entity->render(renderer);
        }
    }

    void EntityManager::addEntity(std::unique_ptr<Entity> entity) {
        m_entities.push_back(std::move(entity));
    }

    void EntityManager::clear() {
        m_entities.clear();
    }
}
