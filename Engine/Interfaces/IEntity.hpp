#pragma once

namespace Engine {
namespace Interfaces {

    class IEntity {
    public:
        virtual ~IEntity() = default;

        virtual void update(float deltaTime, class IMap* map) = 0;
        virtual void render(class IRenderer* renderer) = 0;
    };

}
}
