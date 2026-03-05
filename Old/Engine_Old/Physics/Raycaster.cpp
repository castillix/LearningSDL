#include "Engine/Physics/Raycaster.hpp"
#include "Engine/Game/VoxelMap.hpp"
#include <cmath>

namespace Physics {
    // 3D DDA (Digital Differential Analyzer) Algorithm
    RaycastResult Raycaster::raycast(Game::VoxelMap* map, const glm::vec3& start, const glm::vec3& dir, float maxDistance) {
        RaycastResult result = { false, glm::vec3(0), glm::vec3(0), glm::ivec3(0) };
        if (!map) return result;

        glm::vec3 rayDir = glm::normalize(dir);
        
        // Initial voxel
        int x = (int)floor(start.x);
        int y = (int)floor(start.y);
        int z = (int)floor(start.z);

        // Direction to step
        int stepX = (rayDir.x > 0) ? 1 : ((rayDir.x < 0) ? -1 : 0);
        int stepY = (rayDir.y > 0) ? 1 : ((rayDir.y < 0) ? -1 : 0);
        int stepZ = (rayDir.z > 0) ? 1 : ((rayDir.z < 0) ? -1 : 0);

        // Distance to move one voxel size
        float tDeltaX = (stepX != 0) ? std::abs(1.0f / rayDir.x) : std::numeric_limits<float>::max();
        float tDeltaY = (stepY != 0) ? std::abs(1.0f / rayDir.y) : std::numeric_limits<float>::max();
        float tDeltaZ = (stepZ != 0) ? std::abs(1.0f / rayDir.z) : std::numeric_limits<float>::max();

        // Distance accumulated so far
        float tMaxX = (stepX > 0) ? (floor(start.x) + 1.0f - start.x) * tDeltaX : (start.x - floor(start.x)) * tDeltaX;
        float tMaxY = (stepY > 0) ? (floor(start.y) + 1.0f - start.y) * tDeltaY : (start.y - floor(start.y)) * tDeltaY;
        float tMaxZ = (stepZ > 0) ? (floor(start.z) + 1.0f - start.z) * tDeltaZ : (start.z - floor(start.z)) * tDeltaZ;

        float distance = 0.0f;
        glm::vec3 normal(0.0f);

        while (distance <= maxDistance) {
            // Check current block
            if (map->getBlock(x, y, z) != 0) {
                result.hit = true;
                result.hitPos = start + rayDir * distance;
                result.hitNormal = normal;
                result.blockPos = glm::ivec3(x, y, z);
                return result;
            }

            // Step along ray
            if (tMaxX < tMaxY) {
                if (tMaxX < tMaxZ) {
                    x += stepX;
                    distance = tMaxX;
                    tMaxX += tDeltaX;
                    normal = glm::vec3(-stepX, 0, 0);
                } else {
                    z += stepZ;
                    distance = tMaxZ;
                    tMaxZ += tDeltaZ;
                    normal = glm::vec3(0, 0, -stepZ);
                }
            } else {
                if (tMaxY < tMaxZ) {
                    y += stepY;
                    distance = tMaxY;
                    tMaxY += tDeltaY;
                    normal = glm::vec3(0, -stepY, 0);
                } else {
                    z += stepZ;
                    distance = tMaxZ;
                    tMaxZ += tDeltaZ;
                    normal = glm::vec3(0, 0, -stepZ);
                }
            }
        }

        return result;
    }
}
