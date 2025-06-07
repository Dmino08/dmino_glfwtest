#include "core/Time.hpp"

namespace core {

    void Timer::update(float delta) {
        elapsed += delta;

        if (elapsed >= finish_time)
        {
            time_out();
            elapsed = 0.0f;
        }
    }

    Time::Time() {
        last_ = clock::now();
    }
    void Time::update() {
        auto now = clock::now();
        delta_time_ = std::chrono::duration<float>(now - last_).count();
        last_ = now;

        for (auto &timer : timers_)
        {
            timer.update(delta_time_);
        }
        
    }
    float Time::getDeltaTime() {
        return delta_time_;
    }

    void Time::addTimer(Timer&& timer) {
        timers_.push_back(std::move(timer));
    }

}