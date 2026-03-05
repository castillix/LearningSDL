#pragma once
#include "Engine/Interfaces/IRaycaster.hpp"

namespace Engine {
namespace Physics {

    class VoxelRaycaster : public Interfaces::IRaycaster {
    public:
        VoxelRaycaster() = default;
        ~VoxelRaycaster() override = default;

        Interfaces::RaycastResult raycast(Interfaces::IMap* map, glm::vec3 origin, glm::vec3 direction, float maxDistance) const override;
    };

}
}
