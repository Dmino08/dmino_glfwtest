#include "graphics/ui/Canvas.hpp"

     
Canvas::Canvas(Window& window) 
        : window_(window),
          indices_count_(0),
          vertex_count_(0),
          indices_(),
          vertices_(),
          mesh_(),
          ui_camera_(),
          ui_shader_()
{}


void Canvas::init() {
    ui_shader_ = Shader::create("res/shaders/ui.glslv", "res/shaders/ui.glslf");

}

void Canvas::begin() {
    ui_shader_.use();
}

void Canvas::end() {


    mesh_.createMesh(MeshData{vertices_.data(), indices_.data()}, GL_DYNAMIC_DRAW);
    mesh_.draw();
}