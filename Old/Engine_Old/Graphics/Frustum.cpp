#include "Engine/Graphics/Frustum.hpp"

namespace Graphics {
    void Frustum::extractPlanes(const glm::mat4& viewProj) {
        // Left
        planes[0].x = viewProj[0][3] + viewProj[0][0];
        planes[0].y = viewProj[1][3] + viewProj[1][0];
        planes[0].z = viewProj[2][3] + viewProj[2][0];
        planes[0].w = viewProj[3][3] + viewProj[3][0];
        // Right
        planes[1].x = viewProj[0][3] - viewProj[0][0];
        planes[1].y = viewProj[1][3] - viewProj[1][0];
        planes[1].z = viewProj[2][3] - viewProj[2][0];
        planes[1].w = viewProj[3][3] - viewProj[3][0];
        // Bottom
        planes[2].x = viewProj[0][3] + viewProj[0][1];
        planes[2].y = viewProj[1][3] + viewProj[1][1];
        planes[2].z = viewProj[2][3] + viewProj[2][1];
        planes[2].w = viewProj[3][3] + viewProj[3][1];
        // Top
        planes[3].x = viewProj[0][3] - viewProj[0][1];
        planes[3].y = viewProj[1][3] - viewProj[1][1];
        planes[3].z = viewProj[2][3] - viewProj[2][1];
        planes[3].w = viewProj[3][3] - viewProj[3][1];
        // Near
        planes[4].x = viewProj[0][3] + viewProj[0][2];
        planes[4].y = viewProj[1][3] + viewProj[1][2];
        planes[4].z = viewProj[2][3] + viewProj[2][2];
        planes[4].w = viewProj[3][3] + viewProj[3][2];
        // Far
        planes[5].x = viewProj[0][3] - viewProj[0][2];
        planes[5].y = viewProj[1][3] - viewProj[1][2];
        planes[5].z = viewProj[2][3] - viewProj[2][2];
        planes[5].w = viewProj[3][3] - viewProj[3][2];

        // Normalize
        for (int i = 0; i < 6; i++) {
            float length = glm::length(glm::vec3(planes[i]));
            planes[i] /= length;
        }
    }

    bool Frustum::isBoxInFrustum(const Physics::AABB& box) const {
        for (int i = 0; i < 6; i++) {
            glm::vec3 pos;
            pos.x = (planes[i].x > 0) ? box.maxExtents.x : box.minExtents.x;
            pos.y = (planes[i].y > 0) ? box.maxExtents.y : box.minExtents.y;
            pos.z = (planes[i].z > 0) ? box.maxExtents.z : box.minExtents.z;

            float distance = planes[i].x * pos.x + planes[i].y * pos.y + planes[i].z * pos.z + planes[i].w;
            if (distance < 0) {
                return false;
            }
        }
        return true;
    }
}
