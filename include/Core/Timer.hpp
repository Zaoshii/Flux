#pragma once
#include <chrono>
namespace Engine::Core
{
    class Timer
{
private:
    std::chrono::steady_clock::time_point m_StartPoint;
    std::chrono::steady_clock::time_point m_LastPoint;

public:
    Timer() { Reset(); }

    ~Timer() = default;

    void Reset()
    {
        m_StartPoint = std::chrono::steady_clock::now();
        m_LastPoint = m_StartPoint;
    }

    float DeltaTime()
    {
        auto now = std::chrono::steady_clock::now();
        auto elapsedTime = std::chrono::duration<float>(now - m_LastPoint);
        m_LastPoint = now;
        float dt = elapsedTime.count();
        if (dt > 0.05f)
            dt = 0.006f; // if dt too long consider basic 165 fps frame
        return dt;
    }

    float TotalTime()
    {
        auto now = std::chrono::steady_clock::now();
        auto totalTime = std::chrono::duration<float>(now - m_StartPoint);
        return totalTime.count();
    }
};
}