#pragma once



#include "glad/glad.h"

class Shader;

class Mesh {
    unsigned int VAO, VBO;

    public:
        Mesh(float* vertices, size_t count);

        void draw();
};