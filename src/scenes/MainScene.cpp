#include "scenes/MainScene.hpp"

#include "window/Window.hpp"
#include "window/InputManager.hpp"

#include "graphics/core/Image.hpp"
#include "graphics/core/Texture.hpp"
#include "graphics/glsl/GLSLStructures.hpp"
#include "core/Time.hpp"

#include <iostream>

void MainScene::init(Window& wind) {
    core::Time time;
    time.update();
    // CREATING FRAMEBUFFER
    fbo = makeU<FrameBuffer>();
    fbo->create(wind.getWidth(), wind.getHeight());

    window = &wind;
    input = &wind.getInput();
    glfwSwapInterval(0);

    // CAMERA GENERATION
    CameraParams c_params;
    c_params.z_far = 15000.0f;
    camera = makeU<Camera>(wind, c_params);

    // SHADER GENERATION
    screen_shader = makeU<Shader>();
    screen_shader->create("res/shaders/framebuffer.vert", "res/shaders/framebuffer.frag");

    mult_shader = makeU<Shader>();
    mult_shader->create("res/shaders/light_test.vert","res/shaders/multiple_lights.frag");

    skybox_shader = makeU<Shader>();
    skybox_shader->create("res/shaders/sky_box.vert", "res/shaders/sky_box.frag");

    reflect_shader = makeU<Shader>();
    reflect_shader->create("res/shaders/reflect.vert", "res/shaders/reflect.frag");

    // mult_shader->use();

    // IMAGE GENERATION
    image0 = makeS<Image>();
    image0->load("res/images/Prototype_Grid_Gray_03-512x512.png");

    // TEXTURE GENERATION
    TextureParams tParams;
    tParams.min_filter = GL_LINEAR_MIPMAP_LINEAR;
    tParams.internal_format = GL_RGBA;
    texture0.create(*image0, tParams);

    // FLOOR GENERATION
    const float floor_size = 10000.0f;
    floor = makeU<Sprite>();
    floor->setTexture(&texture0);
    floor->transform.setPosition(glm::vec3(0.0f, -1.0f, 0.0f));
    floor->transform.setScale(glm::vec3(floor_size));
    floor->transform.rotate(glm::vec3(-90.0f, 0.0f, 0.0f));
    floor->setRegion(0,0, 512 * int(floor_size), 512 * int(floor_size));
    floor->generate();

    // LIGHT GENERATION  DIRECTIONAL
    glsl::DirectionalLight direction_light;
    direction_light.base.ambient = glm::vec3(0.08f);
    direction_light.base.diffuse = glm::vec3(0.4f, 0.4f, 0.4f);
    direction_light.base.specular = glm::vec3(0.5f, 0.5f, 0.5f);
    direction_light.direction = glm::vec3(0.0f, -1.0f, 0.0f);

    // LIGHT GENERATION  POINT
    glsl::PointLight point_light;
    point_light.base.ambient = glm::vec3(0.05f, 0.05f, 0.05f);
    point_light.base.diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
    point_light.base.specular = glm::vec3(1.0f, 1.0f, 1.0f);
    point_light.attenuation.constant = 1.0f;
    point_light.attenuation.linear = 0.09f;
    point_light.attenuation.quadratic = 0.032f;
    point_light.position = glm::vec3(5.0f, 6.0f, 7.0f);

    // LIGHT GENERATION  SPOT
    glsl::SpotLight spot_light;
    spot_light.base.ambient = glm::vec3(0.0f, 0.0f, 0.0f);
    spot_light.base.diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
    spot_light.base.specular = glm::vec3(1.0f, 1.0f, 1.0f);
    spot_light.cut_off = 7.5f;
    spot_light.outer_cut_off = 9.5f;
    spot_light.attenuation.constant = 1.0f;
    spot_light.attenuation.linear = 0.009f;
    spot_light.attenuation.quadratic = 0.0032f;
    spot_light.position = glm::vec3(0.0f, 0.0f, 0.0f);
    spot_light.direction = glm::vec3(0.0f, -1.0f, 0.0f);

    // CUBE MAP STUFF
    std::array<std::string, 6> cube_paths = {
        "res/images/skybox/right.jpg",
        "res/images/skybox/left.jpg",
        "res/images/skybox/top.jpg",
        "res/images/skybox/bottom.jpg",
        "res/images/skybox/front.jpg",
        "res/images/skybox/back.jpg"
    };
    cube_map = makeU<CubeMap>();
    cube_map->create(cube_paths);

    //TEXTURE BIDNING
    texture0.activeUnit(1);
    texture0.bind();    
    cube_map->activeUnit(2);
    cube_map->bind();

    // SHADER SET UP
        // MULTISADER
    mult_shader->use();
    glsl::setMaterial(*mult_shader, "material", glsl::Material{0, 1, 32.0f});
    glsl::setDirectionalLight(*mult_shader, "direction_light", direction_light);
    glsl::setSpotLight(*mult_shader, "spot_light", spot_light);
    glsl::setPointLight(*mult_shader, "point_lights[0]", point_light);
    mult_shader->uniform1i("on_flash_light", true);
    mult_shader->uniform1i("use_specular_map", false);
    mult_shader->uniform1i("blinn", true);
        //SKYBOXSHADER
    skybox_shader->use();
    skybox_shader->uniform1i("skybox", cube_map->getUnitId());
        // REFLECTSHADER
    reflect_shader->use();
    reflect_shader->uniform1i("skybox", cube_map->getUnitId());



    skybox = makeU<Voxel>(glm::vec3(0.0f));
    reflectbox = makeU<Voxel>(glm::vec3(0.0f, 5.0f, 0.0f));

    
    model = makeU<modload::Model>();
    model->create("res/models/backpack/backpack.obj");

    fbo->setUnitSlot();
    screen_shader->use();
    screen_shader->uniform1i("screen_texture", fbo->getUnitSlot());

    time.update();
    core::logger.log(core::Logger::INFO, "Init time: " + std::to_string(time.getDeltaTime()));
}



void MainScene::update(float delta) {

    fpsTimer += delta;
    if (fpsTimer >= 1.0f) {
        fpsTimer = 0.0f;
        std::string title = "FPS: " + std::to_string(int(1.0f / delta));
        window->setTitle(title);
    }

    if (window->isResized()) {
            camera->updateProjection();
            fbo->resize(window->getWidth(), window->getHeight());
    }
    if (input->justPressed(GLFW_KEY_TAB)) {
        window->toggleCursor();
    }
    if (input->isCursorLocked()) {

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
        if (input->justPressed(GLFW_KEY_C))
        {
            onFlashLight = !onFlashLight;

            mult_shader->use();
            mult_shader->uniform1i("on_flash_light", onFlashLight);
        }
        if(input->justPressed(GLFW_KEY_LEFT_SHIFT)) {
            camera_speed *= 3;
        }
        if(input->justReleased(GLFW_KEY_LEFT_SHIFT)) {
            camera_speed /= 3;
        }
        if(input->justPressed(GLFW_KEY_F)) {
            std::cout << "CONTEXT: "<< window->isContext() << std::endl;
            vsync = !vsync;
            glfwSwapInterval(vsync ? 1 : 0);
            std::cout << "VSYNC: "<< vsync << std::endl;
        }  
    }   
    if (input->getScrollDeltaY() > 0.0f){
        camera->toZoom(0.8f, 0.0f, 20.0f);
    }
    else if (input->getScrollDeltaY() < 0.0f){
        camera->toZoom(1.2f, 0.0f, 20.0f);
    }

    if (input->justPressed(GLFW_KEY_I))
    {
        core::logger.log(core::Logger::INFO, std::to_string(Mesh::getDrawCalls()));
    }

    if (input->butJustPressed(GLFW_MOUSE_BUTTON_LEFT))
    {
        engine_.attachSceneToWindow("2", "1");
    }
    
}

void MainScene::draw() {
    Mesh::clearDrawCalls();
    
    fbo->bind();

    glClearColor(0.42, 0.42, 0.6, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    mult_shader->setMatrices(camera->getProjectionMatrix(), camera->getViewMatrix());
    mult_shader->uniform3f("view_pos", camera->getPos());
    mult_shader->uniform3f("spot_light.position", camera->getPos());
    mult_shader->uniform3f("spot_light.direction", camera->getFront());
//
    mult_shader->uniform1i("use_specular_map", false);
    mult_shader->uniform1i("material.diffuse", texture0.getUnitId());  
    mult_shader->uniform1f("material.shininess", 32.0f);
    mult_shader->uniformMatrix("model", floor->transform.getModel());
    floor->draw();


    glm::mat4 md = glm::translate(glm::mat4(1.0f), glm::vec3(5.0f, 4.0f, 5.0f));
    mult_shader->uniform1i("use_specular_map", true);
    mult_shader->uniformMatrix("model", md);
    const auto& meshes = model->getMeshes();
    const auto& mats = model->getMaterials();
    const auto& textures = model->getAllTextures();

    for (size_t i = 0; i < meshes.size(); ++i) {
        int diffuse_id = mats[meshes[i].material_index].diffuse;
        int specular_id = mats[meshes[i].material_index].specular;
        float shininess = mats[meshes[i].material_index].shininess;
        if (diffuse_id >= 0) {
            glActiveTexture(GL_TEXTURE10);
            textures[diffuse_id].bind();
            mult_shader->uniform1i("material.diffuse", 10);
        }
        if (specular_id >= 0) {
            glActiveTexture(GL_TEXTURE11);
            textures[specular_id].bind();
            mult_shader->uniform1i("material.specular", 11);
        }
        mult_shader->uniform1f("material.shininess", shininess);

        meshes[i].mesh.draw();
    }
    


    glDepthFunc(GL_LEQUAL);
    glCullFace(GL_FRONT);
        glm::mat4 view = glm::mat4(glm::mat3(camera->getViewMatrix()));
        skybox_shader->setMatrices(camera->getProjectionMatrix(), view);
        skybox->draw();
    glCullFace(GL_BACK);
    glDepthFunc(GL_LESS);

    
    reflect_shader->setMatrices(camera->getProjectionMatrix(), camera->getViewMatrix());
    reflect_shader->uniform3f("camera_pos", camera->getPos());
//
    reflect_shader->uniformMatrix("model", reflectbox->transform.getModel());
    reflectbox->draw();



//
    fbo->drawScreen(*screen_shader, false); 
}

