#pragma once

#include <vector>
#include "graphics/core/Mesh.hpp"
#include "graphics/core/VertexStructures.hpp"

class Shader;

// struct FrameBufferParams 
// {

// };

class FrameBuffer 
{
public:
    FrameBuffer();
    ~FrameBuffer();
    void create(int width, int height);
    void resize(int width, int height);

    void bind() const;

    void setUnitSlot(int index = 0);

    void drawScreen(const Shader& shader, bool clear = true) const;

    void setVertices(std::vector<ScreenVertex>&& vertices);

    void setMesh(Mesh&& mesh);

    int getUnitSlot() const;
    
private:
    unsigned int framebuffer_, renderbuffer_;
    unsigned int texture_;

    Mesh mesh_;
    std::vector<ScreenVertex> vertices_;

    int unit_;
};