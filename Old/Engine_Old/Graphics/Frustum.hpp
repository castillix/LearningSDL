#pragma once
#include <glm/glm.hpp>
#include "Engine/Physics/AABB.hpp"

namespace Graphics {
    class Frustum {
    public:
        glm::vec4 planes[6];

        void extractPlanes(const glm::mat4& viewProj);
        bool isBoxInFrustum(const Physics::AABB& box) const;
    };
}
