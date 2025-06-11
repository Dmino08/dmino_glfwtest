#include "core/Time.hpp"

#include <cmath>

namespace core {

    void Timer::update(float delta) {
        elapsed += delta;

        if (elapsed >= finish_time)
        {
            time_out();
            elapsed = 0.0f;
        }
    }

    Time::Time() : last_(clock::now()), time_(0.0f), delta_time_(0.0f)  {}
    
    void Time::update() {
        auto now = clock::now();
        delta_time_ = std::chrono::duration<float>(now - last_).count();
        time_ += delta_time_;
        last_ = now;

        for (auto &timer : timers_)
        {
            timer.update(delta_time_);
        }
        
    }
    float Time::getDeltaTime() {
        return delta_time_;
    }
    float Time::getTime() 
    {
        return time_;
    }

    void Time::addTimer(Timer&& timer) {
        timers_.push_back(std::move(timer));
    }

    bool Time::each(float interval)
    {
        return std::fmod(time_, interval) < delta_time_;
    }

}