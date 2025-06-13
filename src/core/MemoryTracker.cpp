#include <new>
#include <cstdlib>
#include <iostream>
#include <iomanip>

uint64_t allocated_memory = 0;
uint64_t deallocated_memory = 0;
uint64_t heap_memory_usage = 0;

void* operator new(size_t size) {
    size_t* memory = static_cast<size_t*>(std::malloc(size + sizeof(size_t)));
    if (!memory) throw std::bad_alloc();
    
    *memory = size; 
    allocated_memory += size;
    heap_memory_usage += (size + sizeof(size_t));
    
    return memory + 1;  
}

void operator delete(void* ptr) noexcept {
    if (!ptr) return;
    
    size_t* real_ptr = static_cast<size_t*>(ptr) - 1;
    size_t size = *real_ptr;
    
    heap_memory_usage -= (size + sizeof(size_t));
    deallocated_memory += size;

    std::free(real_ptr);
}

void operator delete(void* ptr, size_t size) noexcept {
    operator delete(ptr);
}


void print_Alloc_Memory_Byte() {
    std::cout << "Allocated memory: " << allocated_memory << " bytes\n";
}

void print_Alloc_Memory_Kilobyte() {
    std::cout << "Allocated memory: " << allocated_memory / 1024.0f << " KB\n";
}

void print_Alloc_Memory_Megabyte() {
    std::cout << std::fixed << std::setprecision(3);
    std::cout << "Allocated memory: " << allocated_memory / 1024.0f / 1024.0f << " MB\n";
}

void print_Dealloc_Memory_Byte() {
    std::cout << "Deallocated memory: " << deallocated_memory << " bytes\n";
}

void print_Dealloc_Memory_Kilobyte() {
    std::cout << "Deallocated memory: " << deallocated_memory / 1024.0f << " KB\n";
}

void print_Dealloc_Memory_Megabyte() {
    std::cout << std::fixed << std::setprecision(3);
    std::cout << "Deallocated memory: " << deallocated_memory / 1024.0f / 1024.0f << " MB\n";
}

void print_Usage_Memory_Byte() {
    std::cout << "Current heap usage: " << heap_memory_usage << " bytes\n";
}

void print_Usage_Memory_Kilobyte() {

    std::cout << "Current heap usage: " << heap_memory_usage / 1024.0f << " KB\n";
}

void print_Usage_Memory_Megabyte() {
    std::cout << std::fixed << std::setprecision(3);
    std::cout << "Current heap usage: " << heap_memory_usage / 1024.0f / 1024.0f << " MB\n";
}