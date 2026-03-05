#pragma once

namespace Engine {
namespace Interfaces {

    class IEngine {
    public:
        virtual ~IEngine() = default;

        virtual void quit() = 0;
        virtual class IWindow* getWindow() = 0;
        virtual class IRenderer* getRenderer() = 0;
        virtual class IInput* getInput() = 0;
    };

}
}
