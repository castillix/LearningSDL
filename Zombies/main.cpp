#include "Engine/Core/Engine.hpp"
#include "Engine/Graphics/SDLWindow.hpp"
#include "Engine/Graphics/GLRenderer.hpp"
#include "Engine/Core/SDLInput.hpp"
#include "GameStates/TestingState.hpp"
#include <memory>
#include <iostream>

using namespace Engine;
using namespace Engine::Core;
using namespace Engine::Graphics;

int main(int argc, char* argv[]) {
    // 1. Instantiate the concrete implementations via Dependency Injection
    auto window = std::make_unique<SDLWindow>();
    auto renderer = std::make_unique<GLRenderer>();
    auto input = std::make_unique<SDLInput>();

    // 2. Create the core engine
    MainEngine engine(std::move(window), std::move(renderer), std::move(input));

    // 3. Initialize Engine
    if (!engine.init("Zombies - Engine Rewrite", 800, 600)) {
        std::cerr << "Engine failed to initialize." << std::endl;
        return -1;
    }

    // 4. Push our Testing State
    engine.pushState(std::make_unique<TestingState>());

    // 5. Run the Engine Loop!
    engine.run();

    return 0;
}
