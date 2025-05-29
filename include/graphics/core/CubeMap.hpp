#pragma once


#include <array>
#include <string>

#include "typedefs.hpp"

class CubeMap {
    uint id_;
    public:
        CubeMap();
        ~CubeMap();
        void create(std::array<std::string, 6> textures_faces);
        void bind() const;

};