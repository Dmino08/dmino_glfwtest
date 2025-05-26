#pragma once

#include <chrono>



namespace core {
    
    class Time {
        using clock = std::chrono::steady_clock;   
        float deltaTime;
        clock::time_point end;
        
        public:
            Time();
            void update();
            float getDeltaTime();

    }; 

}