#pragma once

#include <chrono>

class Timer {
    using Clock = std::chrono::high_resolution_clock;
public:
    Timer() : start_(Clock::now()) {
    }

    void Tick() {
        start_ = Clock::now();
    }

    auto ElapsedNs() const {
        return (Clock::now() - start_).count();
    }

    auto ElapsedMs() const {
        return std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() - start_).count();
    }

private:
    std::chrono::time_point<Clock> start_;
};
