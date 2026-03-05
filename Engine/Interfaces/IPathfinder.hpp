#pragma once
#include <glm/glm.hpp>

namespace Engine {
namespace Interfaces {

    class IPathfinder {
    public:
        virtual ~IPathfinder() = default;

        // Given a grid location, returns the normalized direction vector to walk
        virtual glm::vec3 getDirectionFrom(glm::ivec3 currentPos) const = 0;
        
        // Tells the pathfinder where to route towards
        virtual void setTarget(glm::ivec3 targetPos) = 0;
        
        // Tells the pathfinder to recalculate fields (should be called sparingly)
        virtual void updateRecalculate(class IMap* map) = 0;
    };

}
}
