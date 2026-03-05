#pragma once
#include <string>

namespace Engine {
namespace Interfaces {

    class IWindow {
    public:
        virtual ~IWindow() = default;

        virtual bool init(const std::string& title, int width, int height) = 0;
        virtual void close() = 0;
        virtual void swapBuffers() = 0;
        virtual void setRelativeMouseMode(bool enabled) = 0;
        virtual void* getNativeWindow() = 0;
    };

}
}
