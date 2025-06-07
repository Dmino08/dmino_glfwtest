#include "window/Camera.hpp"
#include <iostream>

#include <algorithm>

Camera::Camera(Window& window, CameraParams params) :
    window_(window),
    _type(params.type),

    fov_(params.fov),
    zNear_(params.z_near),
    zFar_(params.z_far),
    sensitivity_(params.sensitivity),

    rotation_(glm::mat4(1.0f)),

    position_(glm::vec3(0.0f, 0.0f, 0.0f)),
    front_(glm::vec3(0.0f, 0.0f, -1.0f)),
    up_(glm::vec3(0.0f, 1.0f, 0.0f)),
    dir_(glm::vec3(0.0f, 1.0f, 0.0f)),

    zoom_(1.0f),

    camX_(0.0f),
    camY_(0.0f)

{
    updateProjection();
    updateVectors();
}


void Camera::updateProjection() {      
    if (_type == CameraType::ORTHOGRAPHIC) {

        float halfWidth = (window_.getWidth() / zoom_) / 2.0f;
        float halfHeight = (window_.getHeight() / zoom_) / 2.0f;

        float left = -halfWidth;
        float right = halfWidth;
        float bottom = -halfHeight;
        float top = halfHeight;    
                
        projection_ = glm::ortho(left, right, bottom, top, zNear_, zFar_);
    }
    else if (_type == CameraType::PERSPECTIVE) {
        float aspect = (window_.getHeight() != 0) ? (float)window_.getWidth() / (float)window_.getHeight() : 1.0f;
        float adjustedFov = fov_ / zoom_;
        projection_ = glm::perspective(glm::radians(adjustedFov), aspect, zNear_, zFar_);
    }        
}


void Camera::process3DMouseRotation(double deltaX, double deltaY) {
    camY_ += (-deltaY / float(window_.getHeight())) * sensitivity_;
    camX_ += (-deltaX / float(window_.getHeight())) * sensitivity_;

    if (camY_ < -89.0f){
        camY_ = -89.0f;
    }
    if (camY_ > 89.0f){
        camY_ = 89.0f;
    }

    rotation_ = glm::mat4(1.0f);

    rotate(camY_, camX_, 0.0f);
}

void Camera::translate(glm::vec3 offset) {
    position_ += offset;
}
void Camera::setTransform(glm::vec3 value) {
    position_ = glm::vec3(value);
}

void Camera::rotate(float x, float y, float z) {

    rotation_ = glm::rotate(rotation_, glm::radians(z), glm::vec3(0.0f, 0.0f, 1.0f));
    rotation_ = glm::rotate(rotation_, glm::radians(y), glm::vec3(0.0f, 1.0f, 0.0f));
    rotation_ = glm::rotate(rotation_, glm::radians(x), glm::vec3(1.0f, 0.0f, 0.0f));

    updateVectors();
}

void Camera::toZoom(float deltaZoom, float min, float max) { 
    if (min > max) std::swap(min, max);

    if (_type == CameraType::PERSPECTIVE)
    {
        min = 1.0f;
    }
    float newZoom = zoom_ * deltaZoom;
    zoom_ = std::clamp(newZoom, min, max);
    
    updateProjection();
}

void Camera::setFov(float value) {
    fov_ = value;
    updateProjection();
}

glm::mat4 Camera::getView() const {
    return glm::lookAt(position_, position_ + front_, up_);
}

const glm::mat4& Camera::getProjection() const {
    return projection_;
}

glm::mat4 Camera::getProjView() const {
    return projection_ * glm::lookAt(position_, position_ + front_, up_);
}

const glm::vec3& Camera::getFront() const {
    return front_;
}

const glm::vec3& Camera::getRight() const {
    return right_;
}

const glm::vec3& Camera::getUp() const {
    return up_;
}

const glm::vec3& Camera::getDir() const {
    return dir_;
}

const glm::vec3& Camera::getPos() const {
    return position_;
}
