#include "Engine/Core/Time.hpp"

namespace Core {
    Time::Time() : m_lastTime(0), m_deltaTime(0.0f), m_totalTime(0.0f) {
        m_lastTime = SDL_GetTicks();
    }

    void Time::update() {
        Uint64 currentTime = SDL_GetTicks();
        m_deltaTime = (currentTime - m_lastTime) / 1000.0f;
        m_lastTime = currentTime;
        m_totalTime += m_deltaTime;
    }
}
