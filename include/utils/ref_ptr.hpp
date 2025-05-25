#pragma once

#include <cassert>

namespace util {

    template<typename T>
    class ref_ptr {
        T* obj_;
        public:
            explicit ref_ptr(T* obj) : obj_(obj) {}

            bool valid() {return obj_ != nullptr;}

            T& get() const {
                assert(obj_ != nullptr && "ref_ptr: null pointer dereference");

                return *obj_;
            }
    };

}