#include <SDL3/SDL_main.h>
#include "Engine/Core/Engine.hpp"
#include "Engine/Game/WormsGame.hpp"
#include <memory>

int main(int argc, char* args[]) {
    Core::Engine engine("Worms2D - Modular Engine", 800, 600);
    
    auto game = std::make_unique<Game::WormsGame>();
    
    if (engine.init(std::move(game))) {
        engine.run();
    }

    return 0;
}
