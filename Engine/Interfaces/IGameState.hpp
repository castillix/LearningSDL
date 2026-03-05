#pragma once

namespace Engine {
namespace Interfaces {

    class IGameState {
    public:
        virtual ~IGameState() = default;

        virtual void init(class IEngine* engine) = 0;
        virtual void update(float deltaTime) = 0;
        virtual void render(class IRenderer* renderer) = 0;
        virtual void renderUI() = 0;
        virtual void cleanup() = 0;
    };

}
}
