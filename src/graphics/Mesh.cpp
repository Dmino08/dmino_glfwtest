#include "graphics/Mesh.hpp"
#include "graphics/Shader.hpp"

Mesh::Mesh(float* vertices, size_t size, int* attribute_params, size_t attribute_amount) : VAO(0), VBO(0) {
     
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

    int stride = 0;
    for (size_t i = 0; i < attribute_amount; i++)
    {
        stride += attribute_params[i];
    }

    int pointer = 0;
    for (size_t i = 0; i < attribute_amount; i++)
    {
        glVertexAttribPointer(i, attribute_params[i], GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(pointer * sizeof(float)));
        glEnableVertexAttribArray(i);
        pointer += attribute_params[i];

    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Mesh::draw() {
    glBindVertexArray(VAO);

    glDrawArrays(GL_TRIANGLES, 0, 36);

    glBindVertexArray(0);
}
