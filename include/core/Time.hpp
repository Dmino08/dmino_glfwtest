#pragma once

#include <chrono>
#include <functional>
#include <vector>


namespace core {
    
    struct Timer
    {
    public:
        std::function<void()> time_out;
        float finish_time = 1.0f;
        float elapsed = 0.0f;

        void update(float delta);
    };

    class Time {
    public:
        Time();
        void update();
        float getDeltaTime();
        float getTime();
        
        void addTimer(Timer&& timer);
        bool each(float interval);

        
    private:
        using clock = std::chrono::steady_clock;   
        float delta_time_;
        clock::time_point last_;
        std::vector<Timer> timers_;
        float time_;

    }; 

}