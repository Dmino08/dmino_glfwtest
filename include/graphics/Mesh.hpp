#pragma once



#include <glad/glad.h>


class Mesh {
    unsigned int VAO, VBO;
    size_t vertice_count;

    public:
        Mesh() = default;
        Mesh(float* vertices, 
             size_t size, 
             int* attribute_params,
             size_t attribute_amount
             );

        void draw();
};