#include "core/Time.hpp"

namespace core {

    Time::Time() {
        last = clock::now();
    }
    void Time::update() {
        auto now = clock::now();
        deltaTime = std::chrono::duration<float>(now - last).count();
        last = now;
    }
    float Time::getDeltaTime() {
        return deltaTime;
    }

}