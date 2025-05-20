#pragma once

#include <glm/gtc/matrix_transform.hpp>

#include "window/Window.hpp"

enum CameraType {
    PERSPECTIVE,
    ORTHOGRAPHIC
};

/**
 * @brief Contains configuration parameters for the Camera class
 * 
 * Stores settings like camera type, resolution, field of view, and clipping planes.
 */
struct CameraParams {
    CameraType type = CameraType::PERSPECTIVE;
    float fov = 45.0f;
    float z_near = 0.1f;
    float z_far = 100.0f;
    float sensitivity = 1.0f;
};


class Camera {
    Window& window_;
    
    glm::vec3 front_;
    glm::vec3 right_;
    glm::vec3 up_;
    glm::vec3 dir_;

    glm::vec3 position_;
    
    glm::mat4 projection_;
    glm::mat4 rotation_;

    float fov_;
    float zNear_;
    float zFar_;

    float sensitivity_;

    float zoom_;

    CameraType _type;

    float camX_;
    float camY_;


    
    inline void updateProjection() {     
        
        if (_type == CameraType::ORTHOGRAPHIC) {

            float halfWidth = (window_.getWidth() * zoom_) / 2.0f;
            float halfHeight = (window_.getHeight() * zoom_) / 2.0f;

            float left = -halfWidth;
            float right = halfWidth;
            float bottom = -halfHeight;
            float top = halfHeight;    
                   
            projection_ = glm::ortho(left, right, bottom, top, zNear_, zFar_);
        }
        else if (_type == CameraType::PERSPECTIVE) {
            float aspect = float(window_.getWidth()) / float(window_.getHeight());
            projection_ = glm::perspective(fov_ / zoom_, aspect, zNear_, zFar_);
        }        
    }

    inline void updateVectors() {
        front_ = glm::vec3(rotation_ * glm::vec4(0.0f, 0.0f, -1.0f, 1.0f));
        right_ = glm::vec3(rotation_ * glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
        up_ = glm::vec3(rotation_ * glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
        dir_ = front_;
        dir_.y = 0.0f;
    }

    public:
        Camera(Window& window, CameraParams params);
        ~Camera() = default;

        void process3DMouseRotation(double deltaX, double deltaY);

        void translate(glm::vec3 offset);
        void setTransform(glm::vec3 value);

        void rotate(float x, float y, float z);

        void toZoom(float deltaZoom, float min = 1.0f, float max = 2.0f);

        glm::mat4 getViewMatrix() const;
        const glm::mat4& getProjectionMatrix() const;

        const glm::vec3& getFront() const;
        const glm::vec3& getRight() const;
        const glm::vec3& getUp() const;
        const glm::vec3& getDir() const;
        const glm::vec3& getPos() const;

};