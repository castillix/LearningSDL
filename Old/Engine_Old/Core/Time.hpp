#pragma once
#include <SDL3/SDL.h>

namespace Core {
    class Time {
    public:
        Time();

        void update();

        float getDeltaTime() const { return m_deltaTime; }
        float getTotalTime() const { return m_totalTime; }

    private:
        Uint64 m_lastTime;
        float m_deltaTime;
        float m_totalTime;
    };
}
