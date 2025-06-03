#pragma once


#include <array>
#include <string>
#include <glad/glad.h>

#include "typedefs.hpp"


class CubeMap {
    uint id_;
    int unit_;
    public:
        CubeMap();
        ~CubeMap();
        void create(std::array<std::string, 6> textures_faces, GLint internalformat = GL_RGBA);

        void activeUnit(int index = 0);
        int getUnitId() const;

        void bind() const;

};