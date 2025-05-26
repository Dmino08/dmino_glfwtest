#include "core/Time.hpp"

namespace core {

    Time::Time() {
        end = clock::now();
    }
    void Time::update() {
        auto start = clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(start - end);
        deltaTime = elapsed.count() / 1'000'000.0f;
        end = start;
    }
    float Time::getDeltaTime() {
        return deltaTime;
    }

}