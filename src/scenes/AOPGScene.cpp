#include "scenes/AOPGScene.hpp"

#include "graphics/core/Shader.hpp"
#include "window/Window.hpp"
#include "window/Camera.hpp"
#include "objects/Voxel.hpp"

#include <glm/gtc/type_ptr.hpp>


inline void updateUBOMatrices(Camera* camera, int ubo_buffer) {
    glBindBuffer(GL_UNIFORM_BUFFER, ubo_buffer); \
    glm::mat4 matrices[2] = {camera->getProjectionMatrix(), camera->getViewMatrix()}; \
    glBufferSubData(GL_UNIFORM_BUFFER, 0, 2 * sizeof(glm::mat4), glm::value_ptr(matrices[0])); \
    glBindBuffer(GL_UNIFORM_BUFFER, 0);    
}

void AOPGScene::init(Window& wind) {
    window = &wind;
    input = &wind.getInput();
    
    CameraParams c_params;
    camera = std::make_unique<Camera>(wind, c_params);

    shader = std::make_shared<Shader>();
    shader->create("res/shaders/advanced.vert", "res/shaders/advanced.frag");
    assets.addItem(shader, "advanced");

    voxel = std::make_unique<Voxel>(glm::vec3(0.0f, 0.0f, 0.0f));
    voxel->transform.setPosition(glm::vec3(0.0f ,0.0f, -2.0f));

    // BLOCK BINDING STUFF
    shader->use();
    shader->uniformBlockBinding("Matrices", 0);
    //
    ubo_buffer = *assets.getItem<int>("Matrices");
}   

void AOPGScene::update(float delta) {
    
    fpsTimer += delta;
    if (fpsTimer >= 1.0f) {
        fpsTimer = 0.0f;
        std::string title = "FPS: " + std::to_string(int(1.0f / delta));
        window->setTitle(title);
    }

    camera->process3DMouseRotation(input->getDeltaX(), input->getDeltaY());

    if (input->pressed(GLFW_KEY_W)) {
        camera->translate(camera_speed * camera->getFront() * delta);
    }
    if (input->pressed(GLFW_KEY_S)) {
        camera->translate(-camera_speed * camera->getFront() * delta);
    }
    if (input->pressed(GLFW_KEY_A)) {
        camera->translate(-camera_speed * camera->getRight() * delta);
    }
    if (input->pressed(GLFW_KEY_D)) {
        camera->translate(camera_speed * camera->getRight() * delta);
    }      
    
    
    if (input->butJustPressed(GLFW_MOUSE_BUTTON_LEFT))
    {
        engine_.attachSceneToWindow("1", "1");
    }
}

void AOPGScene::draw() {
    glClearColor(0.42, 0.42, 0.6, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    shader->use();

    updateUBOMatrices(camera.get(), ubo_buffer);

    shader->uniformMatrix("model", voxel->transform.getModel());
    voxel->draw(GL_LINES);
}
