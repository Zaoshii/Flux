#pragma once

#include <chrono>

namespace engine::core
{

class Timer
{

public:
    Timer() { reset(); }

    ~Timer() = default;

    /**
     * @brief restarts entirely the timer
     *
     */
    void reset()
    {
        startPoint_ = std::chrono::steady_clock::now();
        lastPoint_ = startPoint_;
        deltaTime_ = 0.0f; ///< Elapsed time in seconds since last frame
    }

    /**
     * @brief updates deltaTime
     *
     */
    void update()
    {
        auto now = std::chrono::steady_clock::now();

        deltaTime_ = std::chrono::duration<float>(now - lastPoint_).count();

        lastPoint_ = now;
        // if (dt > 0.05f)  // TODO: Important to know : fixed framerate
        // dt = 0.006f; // if dt too long consider basic 165 fps frame
    }

    /**
     *
     *
     * @return deltaTime in seconds
     */
    float getDeltaTime() const { return deltaTime_; }

    float getTotalTime() const
    {
        auto now = std::chrono::steady_clock::now();
        auto totalTime = std::chrono::duration<float>(now - startPoint_);
        return totalTime.count();
    }

private:
    std::chrono::steady_clock::time_point startPoint_;
    std::chrono::steady_clock::time_point lastPoint_;
    float deltaTime_ = 0.0f;
};

} // namespace engine::core