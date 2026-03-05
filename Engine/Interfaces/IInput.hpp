#pragma once

namespace Engine {
namespace Interfaces {

    class IInput {
    public:
        virtual ~IInput() = default;

        virtual void updateBegin() = 0;
        virtual bool isKeyHeld(int keycode) const = 0;
        virtual bool isKeyJustPressed(int keycode) const = 0;
        virtual bool isMouseHeld(int button) const = 0;
        virtual bool isMouseJustPressed(int button) const = 0;
        virtual void getMouseDelta(float& dx, float& dy) const = 0;
        
        // Normally handles internal polling
        virtual void pollEvents(class IEngine* engine) = 0;
    };

}
}
