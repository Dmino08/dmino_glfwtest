#pragma once

#include <chrono>



namespace core {
    
    class Time {
        using clock = std::chrono::steady_clock;   
        float delta_time_;
        clock::time_point last_;
        
        public:
            Time();
            void update();
            float getDeltaTime();

    }; 

}