#pragma once
#include <glm/glm.hpp>
#include <cstdint>

namespace Engine {
namespace Interfaces {

    struct RaycastResult {
        bool hit;
        glm::ivec3 blockPos;
        glm::vec3 hitPoint;
        glm::ivec3 normal;
        uint8_t hitBlockType;
    };

    class IRaycaster {
    public:
        virtual ~IRaycaster() = default;

        // Perform a precise raycast against the world implemented by IMap
        virtual RaycastResult raycast(class IMap* map, glm::vec3 origin, glm::vec3 direction, float maxDistance) const = 0;
    };

}
}
