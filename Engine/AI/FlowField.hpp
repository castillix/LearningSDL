#pragma once
#include "Engine/Interfaces/IPathfinder.hpp"
#include <unordered_map>
#include <queue>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

namespace Engine {
namespace AI {

    class FlowField : public Interfaces::IPathfinder {
    public:
        FlowField() = default;
        ~FlowField() override = default;

        glm::vec3 getDirectionFrom(glm::ivec3 currentPos) const override;
        void setTarget(glm::ivec3 targetPos) override;
        void updateRecalculate(Interfaces::IMap* map) override;

    private:
        glm::ivec3 m_targetPos;
        std::unordered_map<glm::ivec3, int> m_costField;
        std::unordered_map<glm::ivec3, glm::vec3> m_flowField;

        void generateCostField(Interfaces::IMap* map);
        void generateFlowField();
    };

}
}
