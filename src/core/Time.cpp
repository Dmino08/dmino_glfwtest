#include "core/Time.hpp"

namespace core {

    Time::Time() {
        last_ = clock::now();
    }
    void Time::update() {
        auto now = clock::now();
        delta_time_ = std::chrono::duration<float>(now - last_).count();
        last_ = now;
    }
    float Time::getDeltaTime() {
        return delta_time_;
    }

}