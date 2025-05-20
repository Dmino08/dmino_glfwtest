#include "core/Logger.hpp"

#include <filesystem>

namespace fs = std::filesystem;

Logger logger;


Logger::Logger() {

    std::time_t t = std::time(nullptr);
    std::strftime(launch_time_, sizeof(launch_time_), "%Y-%m-%d %H-%M-%S", std::localtime(&t));
    
    fs::create_directories("logs");

    log_.open("logs/Log.txt");
    current_log_.open("logs/Log[" + std::string(launch_time_) + "].txt");
    
}

Logger::~Logger() {
    log_.close();
    current_log_.close();
}


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

    std::string text = "[" + level_str + "][" + buf + "]: " + message;

    log_ << text << std::endl;
    current_log_ << text << std::endl;

    std::cout << text << std::endl;
}

