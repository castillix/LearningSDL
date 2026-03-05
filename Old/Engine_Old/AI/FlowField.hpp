#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <queue>
#include "Engine/Game/VoxelMap.hpp"

namespace AI {
    class FlowField {
    public:
        FlowField(int width, int height, int depth);

        // Generate the flow field pointing to targetPos
        void generate(Game::VoxelMap* map, const glm::ivec3& targetPos);

        // Get the direction to move from a specific world position
        glm::vec3 getDirection(const glm::vec3& worldPos) const;

    private:
        int m_width, m_height, m_depth;
        glm::ivec3 m_offset; // World position of the bottom-left-back corner of the field

        std::vector<uint16_t> m_costField;
        std::vector<uint16_t> m_integrationField;
        std::vector<glm::vec3> m_vectorField;

        int getIndex(int x, int y, int z) const {
            return x + (y * m_width) + (z * m_width * m_height);
        }
        
        bool inBounds(int x, int y, int z) const {
            return x >= 0 && x < m_width && y >= 0 && y < m_height && z >= 0 && z < m_depth;
        }
    };
}
