#pragma once

namespace Engine {
namespace Interfaces {

    class IRenderer {
    public:
        virtual ~IRenderer() = default;

        virtual bool init(void* nativeWindow) = 0;
        virtual void close() = 0;
        virtual void clear(float r, float g, float b, float a) = 0;
        
        // Additional methods will be added for rendering specific items
        // but for now, we just define the core lifecycle and clearing.
    };

}
}
