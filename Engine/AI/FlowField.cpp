#include "Engine/AI/FlowField.hpp"
#include "Engine/Interfaces/IMap.hpp"
#include <limits>

namespace Engine {
namespace AI {

    using namespace Interfaces;

    const int SEARCH_RADIUS = 32;

    glm::vec3 FlowField::getDirectionFrom(glm::ivec3 currentPos) const {
        auto it = m_flowField.find(currentPos);
        if (it != m_flowField.end()) {
            return it->second;
        }
        return glm::vec3(0.0f);
    }

    void FlowField::setTarget(glm::ivec3 targetPos) {
        m_targetPos = targetPos;
    }

    void FlowField::updateRecalculate(IMap* map) {
        if (!map) return;
        m_costField.clear();
        m_flowField.clear();

        generateCostField(map);
        generateFlowField();
    }

    void FlowField::generateCostField(IMap* map) {
        std::queue<glm::ivec3> frontier;
        frontier.push(m_targetPos);
        m_costField[m_targetPos] = 0;

        glm::ivec3 ds[] = {
            {1,0,0}, {-1,0,0}, {0,0,1}, {0,0,-1},
            {1,0,1}, {-1,0,1}, {1,0,-1}, {-1,0,-1},
            {1,1,0}, {-1,1,0}, {0,1,1}, {0,1,-1}, {0,1,0},
            {1,-1,0}, {-1,-1,0}, {0,-1,1}, {0,-1,-1}, {0,-1,0}
        };

        while (!frontier.empty()) {
            glm::ivec3 current = frontier.front();
            frontier.pop();

            int currentCost = m_costField[current];

            if (glm::distance((glm::vec3)current, (glm::vec3)m_targetPos) > SEARCH_RADIUS) {
                continue;
            }

            for (const auto& d : ds) {
                glm::ivec3 neighbor = current + d;

                // Simple check: is neighbor passable? (block is not solid)
                if (!map->isSolid(neighbor.x, neighbor.y, neighbor.z) &&
                    !map->isSolid(neighbor.x, neighbor.y + 1, neighbor.z)) {
                    
                    int newCost = currentCost + 1; // Unweighted distance
                    if (m_costField.find(neighbor) == m_costField.end() || newCost < m_costField[neighbor]) {
                        m_costField[neighbor] = newCost;
                        frontier.push(neighbor);
                    }
                }
            }
        }
    }

    void FlowField::generateFlowField() {
        glm::ivec3 ds[] = {
            {1,0,0}, {-1,0,0}, {0,0,1}, {0,0,-1},
            {1,0,1}, {-1,0,1}, {1,0,-1}, {-1,0,-1},
            {1,1,0}, {-1,1,0}, {0,1,1}, {0,1,-1}, {0,1,0},
            {1,-1,0}, {-1,-1,0}, {0,-1,1}, {0,-1,-1}, {0,-1,0}
        };

        for (const auto& pair : m_costField) {
            glm::ivec3 current = pair.first;
            int currentCost = pair.second;

            if (currentCost == 0) {
                m_flowField[current] = glm::vec3(0.0f);
                continue;
            }

            int bestCost = currentCost;
            glm::ivec3 bestDir(0);

            for (const auto& d : ds) {
                glm::ivec3 neighbor = current + d;
                auto it = m_costField.find(neighbor);
                if (it != m_costField.end() && it->second < bestCost) {
                    bestCost = it->second;
                    bestDir = d;
                }
            }

            if (glm::length((glm::vec3)bestDir) > 0.0f) {
                m_flowField[current] = glm::normalize((glm::vec3)bestDir);
            } else {
                m_flowField[current] = glm::vec3(0.0f);
            }
        }
    }

}
}
