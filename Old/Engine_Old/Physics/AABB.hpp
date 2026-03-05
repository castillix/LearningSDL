#pragma once
#include <glm/glm.hpp>

namespace Physics {
    class AABB {
    public:
        glm::vec3 minExtents;
        glm::vec3 maxExtents;

        AABB() : minExtents(0.0f), maxExtents(0.0f) {}
        AABB(const glm::vec3& min, const glm::vec3& max) : minExtents(min), maxExtents(max) {}

        bool intersects(const AABB& other) const {
            return (minExtents.x < other.maxExtents.x && maxExtents.x > other.minExtents.x) &&
                   (minExtents.y < other.maxExtents.y && maxExtents.y > other.minExtents.y) &&
                   (minExtents.z < other.maxExtents.z && maxExtents.z > other.minExtents.z);
        }

        AABB offset(const glm::vec3& offsetVec) const {
            return AABB(minExtents + offsetVec, maxExtents + offsetVec);
        }
    };
}
