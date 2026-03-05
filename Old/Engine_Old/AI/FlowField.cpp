#include "Engine/AI/FlowField.hpp"

namespace AI {
    FlowField::FlowField(int width, int height, int depth)
        : m_width(width), m_height(height), m_depth(depth) {
        int size = width * height * depth;
        m_costField.resize(size, 1);
        m_integrationField.resize(size, 65535);
        m_vectorField.resize(size, glm::vec3(0.0f));
    }

    void FlowField::generate(Game::VoxelMap* map, const glm::ivec3& targetPos) {
        // Center the flow field around the target
        m_offset = targetPos - glm::ivec3(m_width/2, m_height/2, m_depth/2);
        
        int size = m_width * m_height * m_depth;
        std::fill(m_costField.begin(), m_costField.end(), 1);
        std::fill(m_integrationField.begin(), m_integrationField.end(), 65535);
        std::fill(m_vectorField.begin(), m_vectorField.end(), glm::vec3(0.0f));

        // 1. Generate Cost Field from map (solid blocks are impassable)
        for (int x = 0; x < m_width; ++x) {
            for (int y = 0; y < m_height; ++y) {
                for (int z = 0; z < m_depth; ++z) {
                    glm::ivec3 worldP = m_offset + glm::ivec3(x, y, z);
                    uint8_t block = map->getBlock(worldP.x, worldP.y, worldP.z);
                    if (block != 0) {
                        m_costField[getIndex(x, y, z)] = 255; // Impassable
                    } else {
                        // Check if block below is solid so they can walk, else cost is higher (falling)
                        uint8_t floor = map->getBlock(worldP.x, worldP.y - 1, worldP.z);
                        if (floor == 0) m_costField[getIndex(x, y, z)] = 10; // High cost for falling
                    }
                }
            }
        }

        // 2. Integration Field (Dijkstra's)
        std::vector<glm::ivec3> openSet;
        openSet.reserve(size * 2); // Avoid frequent reallocations
        int head = 0;
        
        glm::ivec3 localTarget = targetPos - m_offset;
        if (inBounds(localTarget.x, localTarget.y, localTarget.z)) {
            m_integrationField[getIndex(localTarget.x, localTarget.y, localTarget.z)] = 0;
            openSet.push_back(localTarget);
        } else {
            return; // Target outside of field
        }

        glm::ivec3 neighbors[6] = {
            glm::ivec3(1,0,0), glm::ivec3(-1,0,0),
            glm::ivec3(0,1,0), glm::ivec3(0,-1,0),
            glm::ivec3(0,0,1), glm::ivec3(0,0,-1)
        };

        while (head < openSet.size()) {
            glm::ivec3 cur = openSet[head++];

            uint16_t curCost = m_integrationField[getIndex(cur.x, cur.y, cur.z)];

            for (const auto& offset : neighbors) {
                glm::ivec3 nb = cur + offset;
                if (!inBounds(nb.x, nb.y, nb.z)) continue;
                
                int idx = getIndex(nb.x, nb.y, nb.z);
                uint8_t cost = m_costField[idx];
                if (cost == 255) continue; // Impassable

                if (curCost + cost < m_integrationField[idx]) {
                    m_integrationField[idx] = curCost + cost;
                    openSet.push_back(nb);
                }
            }
        }

        // 3. Vector Field
        for (int x = 0; x < m_width; ++x) {
            for (int y = 0; y < m_height; ++y) {
                for (int z = 0; z < m_depth; ++z) {
                    int idx = getIndex(x, y, z);
                    if (m_costField[idx] == 255) continue; // Solid block has no vectors

                    uint16_t minCost = m_integrationField[idx];
                    glm::vec3 bestDir(0.0f);

                    // Look around for lowest cost
                    for (int dx = -1; dx <= 1; ++dx) {
                        for (int dy = -1; dy <= 1; ++dy) {
                            for (int dz = -1; dz <= 1; ++dz) {
                                if (dx == 0 && dy == 0 && dz == 0) continue;
                                glm::ivec3 nb(x + dx, y + dy, z + dz);
                                if (!inBounds(nb.x, nb.y, nb.z)) continue;

                                uint16_t nbCost = m_integrationField[getIndex(nb.x, nb.y, nb.z)];
                                if (nbCost < minCost) {
                                    minCost = nbCost;
                                    bestDir = glm::normalize(glm::vec3(dx, dy, dz));
                                }
                            }
                        }
                    }
                    m_vectorField[idx] = bestDir;
                }
            }
        }
    }

    glm::vec3 FlowField::getDirection(const glm::vec3& worldPos) const {
        glm::ivec3 localPos = glm::ivec3(std::floor(worldPos.x), std::floor(worldPos.y), std::floor(worldPos.z)) - m_offset;
        
        if (!inBounds(localPos.x, localPos.y, localPos.z)) {
            return glm::vec3(0.0f); // Fallback to no movement if out of bounds
        }
        
        return m_vectorField[getIndex(localPos.x, localPos.y, localPos.z)];
    }
}
