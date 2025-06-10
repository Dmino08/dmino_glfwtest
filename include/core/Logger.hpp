#pragma once

#include <iostream>
#include <string>
#include <ctime>
#include <fstream>

#define DEBUG_MODE
#define MEMORY_DEBUG

#ifdef MEMORY_DEBUG
    extern int allocated_memory;
    extern int deallocated_memory;
    extern int heap_memory_usage;

    inline void* operator new(size_t n)
    {
        if (n == 0)
        {
            n = 1;
        }

        void* ptr;

        while (true)
        {
            ptr = malloc(n);

            if (ptr)
            {
                allocated_memory += n;
                heap_memory_usage += n;
                return ptr;
            }
            
            std::new_handler handler = std::get_new_handler();
            if (!handler)
            {
                throw std::bad_alloc();
            }

            (*handler)();
        }
    }

    inline void operator delete(void* ptr, size_t n)
    {
        heap_memory_usage -= n;
        deallocated_memory += n;
    }

    void print_Alloc_Memory_Byte();
    void print_Alloc_Memory_Kilobyte();
    void print_Alloc_Memory_Megabyte();

    void print_Dealloc_Memory_Byte();
    void print_Dealloc_Memory_Kilobyte();
    void print_Dealloc_Memory_Megabyte();

    void print_Usage_Memory_Byte();
    void print_Usage_Memory_Kilobyte();
    void print_Usage_Memory_Megabyte();

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