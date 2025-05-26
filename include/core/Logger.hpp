#pragma once

#include <iostream>
#include <string>
#include <ctime>
#include <fstream>

#define DEBUG_MOD

#ifdef DEBUG_MOD
    #define debugPrint(x) std::cout << x << std::endl
#endif

namespace core {

class Logger {
    std::ofstream current_log_;
    std::ofstream log_;
    char launch_time_[100];
    
    public:
        Logger();
        ~Logger();
        enum LogLevel {INFO, WARNING, ERROR};
        void log(LogLevel level, std::string message);

};

extern Logger logger;

}