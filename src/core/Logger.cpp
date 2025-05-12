#include "core/Logger.hpp"

Logger logger;


void Logger::log(LogLevel level, std::string message) {
    std::string level_str;
    switch (level)
    {
        case INFO: level_str = "INFO"; break;
        case WARNING: level_str = "WARNING"; break;
        case ERROR: level_str = "ERROR"; break;
    }
    
    std::time_t t = std::time(nullptr);
    char buf[100];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&t));

    std::cout << "[" << level_str << "]" << "[" << buf << "]: " << message << std::endl;
}

