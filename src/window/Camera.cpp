#include "window/Camera.hpp"




Camera::Camera(CameraParams params) :
    _type(params.type),

    _width(params.width),
    _height(params.height),

    _fov(params.fov),
    _z_near(params.z_near),
    _z_far(params.z_far),
    _sensitivity(params.sensitivity),

    _rotation(glm::mat4(1.0f)),

    _position(glm::vec3(0.0f, 0.0f, 0.0f)),
    _front(glm::vec3(0.0f, 0.0f, -1.0f)),
    _up(glm::vec3(0.0f, 1.0f, 0.0f)),
    _dir(glm::vec3(0.0f, 1.0f, 0.0f)),

    _zoom(1.0f),

    _cam_x(0.0f),
    _cam_y(0.0f)

{
    updateProjection();
    updateVectors();
}

void Camera::process3DMouseRotation(double delta_x, double delta_y) {
    _cam_y += (-delta_y / float(_height)) * _sensitivity;
    _cam_x += (-delta_x / float(_height)) * _sensitivity;

    if (_cam_y < -glm::radians(89.0f)){
        _cam_y = -glm::radians(89.0f);
    }
    if (_cam_y > glm::radians(89.0f)){
        _cam_y = glm::radians(89.0f);
    }

    _rotation = glm::mat4(1.0f);

    rotate(_cam_y, _cam_x, 0.0f);
}

void Camera::translate(glm::vec3 offset) {
    _position += offset;
}
void Camera::setTransform(glm::vec3 new_position) {
    _position = new_position;
}

void Camera::rotate(float x, float y, float z) {

    _rotation = glm::rotate(_rotation, z, glm::vec3(0.0f, 0.0f, 1.0f));
    _rotation = glm::rotate(_rotation, y, glm::vec3(0.0f, 1.0f, 0.0f));
    _rotation = glm::rotate(_rotation, x, glm::vec3(1.0f, 0.0f, 0.0f));

    updateVectors();
}

void Camera::toZoom(float delta_zoom) {
    _zoom += delta_zoom;
    updateProjection();
}

glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(_position, _position + _front, _up);
}

const glm::mat4& Camera::getProjectionMatrix() const {
    return _projection;
}

const glm::vec3& Camera::getFront() const {
    return _front;
}

const glm::vec3& Camera::getRight() const {
    return _right;
}

const glm::vec3& Camera::getUp() const {
    return _up;
}

const glm::vec3& Camera::getDir() const {
    return _dir;
}

const glm::vec3& Camera::getPos() const {
    return _position;
}
