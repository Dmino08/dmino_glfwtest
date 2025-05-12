#pragma once

#include <glm/gtc/matrix_transform.hpp>

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
    int width = 1280, height = 720;
    float fov = 45.0f;
    float z_near = 0.1f;
    float z_far = 100.0f;
    float sensitivity = 1.0f;
};


class Camera {
    glm::vec3 _front;
    glm::vec3 _right;
    glm::vec3 _up;
    glm::vec3 _dir;

    glm::vec3 _position;
    
    glm::mat4 _projection;
    glm::mat4 _rotation;

    float _fov;
    float _z_near;
    float _z_far;

    float _sensitivity;

    float _zoom;

    int _width, _height;
    CameraType _type;

    float _cam_x;
    float _cam_y;

    
    inline void updateProjection() {     
        
        if (_type == CameraType::ORTHOGRAPHIC) {

            float halfWidth = (_width * _zoom) / 2.0f;
            float halfHeight = (_height * _zoom) / 2.0f;

            float left = -halfWidth;
            float right = halfWidth;
            float bottom = -halfHeight;
            float top = halfHeight;    
                   
            _projection = glm::ortho(left, right, bottom, top, _z_near, _z_far);
        }
        else if (_type == CameraType::PERSPECTIVE) {
            float aspect = float(_width) / float(_height);
            _projection = glm::perspective(_fov * _zoom, aspect, _z_near, _z_far);
        }        
    }

    inline void updateVectors() {
        _front = glm::vec3(_rotation * glm::vec4(0.0f, 0.0f, -1.0f, 1.0f));
        _right = glm::vec3(_rotation * glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
        _up = glm::vec3(_rotation * glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
        _dir = _front;
        _dir.y = 0.0f;
    }

    public:
        Camera(CameraParams params);
        ~Camera() = default;

        void process3DMouseRotation(double x_pos, double y_pos);

        void translate(glm::vec3 offset);
        void setTransform(glm::vec3 new_position);

        void rotate(float x, float y, float z);

        void toZoom(float delta_zoom);

        glm::mat4 getViewMatrix() const;
        const glm::mat4& getProjectionMatrix() const;

        const glm::vec3& getFront() const;
        const glm::vec3& getRight() const;
        const glm::vec3& getUp() const;
        const glm::vec3& getDir() const;
        const glm::vec3& getPos() const;

};