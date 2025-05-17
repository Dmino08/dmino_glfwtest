#pragma once

#include <initializer_list>
#include <algorithm>
#include <memory>

#include "typedefs.hpp"


namespace util {

    template <typename T>
    class Buffer {
        std::unique_ptr<T[]> ptr_;
        uint size_;

        public:
            Buffer(uint size) : size_(size) {
                ptr_ = std::make_unique<T[]>(size_);
            }

            Buffer(std::initializer_list<T> list) : size_(list.size()) {
                ptr_ = std::make_unique<T[]>(size_);
                std::copy(list.begin(), list.end(), ptr_.get());
            }

            Buffer(const Buffer<T>& other)  : size_(other.size_) {
                ptr_ = std::make_unique<T[]>(size_);
                std::copy(other.begin(), other.end(), ptr_.get());
            }
            Buffer(Buffer<T>&& other) noexcept : size_(other.size_), ptr_(std::move(other.ptr_)) {
                other.size_ = 0;
            }

            T& operator[](size_t index) { 
                return ptr_[index]; 
            }

            const T& operator[](size_t index) const { 
                return ptr_[index]; 
            }
            
            Buffer<T>& operator=(const Buffer<T>& other) {
                if (this != &other)
                {
                    size_ = other.size_;
                    ptr_ = std::make_unique<T[]>(size_);
                    std::copy(other.begin(), other.end(), ptr_.get());
                }
                return *this;
            }

            Buffer<T>& operator=(Buffer<T>&& other) noexcept {
                if (this != &other)
                {
                    ptr_ = std::move(other.ptr_);
                    size_ = other.size_;
                    other.size_ = 0;
                }
                return *this;
            }

            const T* begin() const {
                return ptr_.get();
            }

            const T* end() const {
                return ptr_.get() + size_;
            }

            T* begin() {
                return ptr_.get();
            }

            T* end() {
                return ptr_.get() + size_;
            }

            inline uint size() const {
                return size_;
            }
            inline uint getByteSize() const {
                return size_ * sizeof(T);
                
            }
    };

}