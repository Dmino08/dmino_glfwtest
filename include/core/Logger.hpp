#pragma once

#include <iostream>
#include <string>
#include <ctime>

class Logger {
    public:
        enum LogLevel {INFO, WARNING, ERROR};
        void log(LogLevel level, std::string message);

};

extern Logger logger;