#include "graphics/core/Canvas.hpp"


Canvas::Canvas(Window& window) 
        : window_(window),
          verticeCount_(0), 
          indiceCount_(0), 
          ui_camera_(window, CameraParams{ORTHOGRAPHIC}) 
{}

void Canvas::init()  {
    shader_ = Shader::create("res/shaders/ui.vert","res/shaders/ui.frag");
    
}

void Canvas::begin() {
    
    shader_->uniformMatrix("projection", ui_camera_.getProjectionMatrix());
    shader_->uniformMatrix("view", glm::mat4(1.0f));
    shader_->uniformMatrix("model", glm::mat4(1.0f));

    data_.vertices.clearCount();
    data_.indices.clearCount();
}

void Canvas::end() {

    shader_->use();
    mesh_.draw();
}

void Canvas::drawRect(float x, float y, float width, float height, const glm::vec3& color) {
    
}