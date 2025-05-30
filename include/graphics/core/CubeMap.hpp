#pragma once


#include <array>
#include <string>

#include "typedefs.hpp"

class CubeMap {
    uint id_;
    int unit_;
    public:
        CubeMap();
        ~CubeMap();
        void create(std::array<std::string, 6> textures_faces);

        void activeUnit(int index = 0);
        int getUnitId() const;

        void bind() const;

};