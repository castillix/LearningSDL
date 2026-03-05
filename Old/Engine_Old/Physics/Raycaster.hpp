#pragma once
#include <glm/glm.hpp>

namespace Game { class VoxelMap; }

namespace Physics {
    struct RaycastResult {
        bool hit;
        glm::vec3 hitPos;
        glm::vec3 hitNormal;
        glm::ivec3 blockPos;
    };

    class Raycaster {
    public:
        // Performs 3D DDA Raycasting against a VoxelMap to find intersection
        static RaycastResult raycast(Game::VoxelMap* map, const glm::vec3& start, const glm::vec3& dir, float maxDistance);
    };
}
