#include "Engine/Graphics/GLRenderer.hpp"
#include <glad/glad.h>
#include <SDL3/SDL.h>

namespace Engine {
namespace Graphics {

    bool GLRenderer::init(void* nativeWindow) {
        // Glad is already loaded by SDLWindow during context creation
        // We just need to setup standard GL states
        
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CCW);
        
        return true;
    }

    void GLRenderer::close() {
        // Any resources to cleanup
    }

    void GLRenderer::clear(float r, float g, float b, float a) {
        glClearColor(r, g, b, a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

}
}
