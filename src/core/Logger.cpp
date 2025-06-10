#include "core/Logger.hpp"

#include <filesystem>

#ifdef MEMORY_DEBUG
    int allocated_memory = 0;
    int deallocated_memory = 0;
    int heap_memory_usage = 0;
    
    ////////////////////////////////////////////////////////////////////////////
    void print_Alloc_Memory_Byte() 
    {
        std::cout << "Allocated memory: " << allocated_memory << std::endl;
    }
    void print_Alloc_Memory_Kilobyte()
    {
        std::cout << "Allocated memory: " << int(allocated_memory / 1000.0) << std::endl;
    }
    void print_Alloc_Memory_Megabyte()
    {
        std::cout << "Allocated memory: " << int(allocated_memory / 1000.0 / 1000.0) << std::endl;
    }

    ///////////////////////////////////////////////////////////////////////////
    void print_Dealloc_Memory_Byte()
    {
        std::cout << "Dellocated memory: " << deallocated_memory << std::endl;
    }
    void print_Dealloc_Memory_Kilobyte()
    {
        std::cout << "Dellocated memory: " << int(deallocated_memory / 1000.0) << std::endl;
    }
    void print_Dealloc_Memory_Megabyte()
    {
        std::cout << "Dellocated memory: " << int(deallocated_memory / 1000.0 / 1000.0) << std::endl;
    }

    ///////////////////////////////////////////////////////////////////////////
    void print_Usage_Memory_Byte()
    {
        std::cout << "Memory Usage: " << heap_memory_usage << std::endl;
    }
    void print_Usage_Memory_Kilobyte()
    {
        std::cout << "Memory Usage: " << int(heap_memory_usage / 1000.0) << std::endl;
    }
    void print_Usage_Memory_Megabyte()
    {
        std::cout << "Memory Usage: " << int(heap_memory_usage / 1000.0 / 1000.0) << std::endl;
    }
#endif


namespace fs = std::filesystem;

namespace core {

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
    
}