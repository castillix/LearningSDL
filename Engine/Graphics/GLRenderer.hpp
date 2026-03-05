#pragma once
#include "Engine/Interfaces/IRenderer.hpp"

namespace Engine {
namespace Graphics {

    class GLRenderer : public Interfaces::IRenderer {
    public:
        GLRenderer() = default;
        ~GLRenderer() override = default;

        bool init(void* nativeWindow) override;
        void close() override;
        void clear(float r, float g, float b, float a) override;
    };

}
}
