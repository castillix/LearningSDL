#include "Engine/Physics/Raycaster.hpp"
#include "Engine/Interfaces/IMap.hpp"
#include <cmath>

namespace Engine {
namespace Physics {

    using namespace Interfaces;

    RaycastResult VoxelRaycaster::raycast(IMap* map, glm::vec3 origin, glm::vec3 direction, float maxDistance) const {
        RaycastResult result = { false, glm::ivec3(0), glm::vec3(0), glm::ivec3(0), 0 };
        if (!map) return result;

        direction = glm::normalize(direction);

        int x = (int)std::floor(origin.x);
        int y = (int)std::floor(origin.y);
        int z = (int)std::floor(origin.z);

        int stepX = (direction.x > 0) ? 1 : ((direction.x < 0) ? -1 : 0);
        int stepY = (direction.y > 0) ? 1 : ((direction.y < 0) ? -1 : 0);
        int stepZ = (direction.z > 0) ? 1 : ((direction.z < 0) ? -1 : 0);

        float tDeltaX = (stepX != 0) ? std::abs(1.0f / direction.x) : INFINITY;
        float tDeltaY = (stepY != 0) ? std::abs(1.0f / direction.y) : INFINITY;
        float tDeltaZ = (stepZ != 0) ? std::abs(1.0f / direction.z) : INFINITY;

        float tMaxX = (stepX > 0) ? (std::floor(origin.x) + 1.0f - origin.x) * tDeltaX : (origin.x - std::floor(origin.x)) * tDeltaX;
        float tMaxY = (stepY > 0) ? (std::floor(origin.y) + 1.0f - origin.y) * tDeltaY : (origin.y - std::floor(origin.y)) * tDeltaY;
        float tMaxZ = (stepZ > 0) ? (std::floor(origin.z) + 1.0f - origin.z) * tDeltaZ : (origin.z - std::floor(origin.z)) * tDeltaZ;

        glm::ivec3 normal(0, 0, 0);

        for (int i = 0; i < std::ceil(maxDistance) * 3; ++i) { // Safety ceiling
            uint8_t block = map->getBlock(x, y, z);
            if (block != 0) {
                result.hit = true;
                result.blockPos = glm::ivec3(x, y, z);
                result.normal = normal;
                result.hitBlockType = block;
                
                // Approximate hit point calculation
                float tHit = 0.0f;
                if (normal.x != 0) tHit = (x + (1 - stepX) / 2.0f - origin.x) / direction.x;
                else if (normal.y != 0) tHit = (y + (1 - stepY) / 2.0f - origin.y) / direction.y;
                else if (normal.z != 0) tHit = (z + (1 - stepZ) / 2.0f - origin.z) / direction.z;
                
                result.hitPoint = origin + direction * tHit;
                return result;
            }

            if (tMaxX < tMaxY) {
                if (tMaxX < tMaxZ) {
                    if (tMaxX > maxDistance) break;
                    x += stepX;
                    tMaxX += tDeltaX;
                    normal = glm::ivec3(-stepX, 0, 0);
                } else {
                    if (tMaxZ > maxDistance) break;
                    z += stepZ;
                    tMaxZ += tDeltaZ;
                    normal = glm::ivec3(0, 0, -stepZ);
                }
            } else {
                if (tMaxY < tMaxZ) {
                    if (tMaxY > maxDistance) break;
                    y += stepY;
                    tMaxY += tDeltaY;
                    normal = glm::ivec3(0, -stepY, 0);
                } else {
                    if (tMaxZ > maxDistance) break;
                    z += stepZ;
                    tMaxZ += tDeltaZ;
                    normal = glm::ivec3(0, 0, -stepZ);
                }
            }
        }

        return result;
    }

}
}
