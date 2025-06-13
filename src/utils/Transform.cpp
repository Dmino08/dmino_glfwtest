#include "utils/Transform.hpp"


namespace util {

    void Transform::updateVectors() {
        model_ = glm::mat4(1.0f);

        model_ = glm::translate(model_, position_);


        model_ = glm::rotate(model_, glm::radians(rotation_.z), glm::vec3(0.0f ,0.0f, 1.0f));
        model_ = glm::rotate(model_, glm::radians(rotation_.y), glm::vec3(0.0f ,1.0f, 0.0f));
        model_ = glm::rotate(model_, glm::radians(rotation_.x), glm::vec3(1.0f ,0.0f, 0.0f));

        model_ = glm::scale(model_, scale_);  
    }



    Transform::Transform() 
    : position_(glm::vec3(0.0f, 0.0f, 0.0f)),
      rotation_(glm::vec3(0.0f, 0.0f, 0.0f)),
      scale_(glm::vec3(1.0f, 1.0f, 1.0f)),
      model_(glm::mat4(1.0f))
    {
        updateVectors();
    }

    Transform::Transform(glm::mat4 matrix)
    : position_(glm::vec3(0.0f, 0.0f, 0.0f)),
      rotation_(glm::vec3(0.0f, 0.0f, 0.0f)), 
      scale_(glm::vec3(1.0f, 1.0f, 1.0f)),
      model_(glm::mat4(1.0f))
    {
        model_ = matrix;
    }

    Transform::Transform(float x, float y, float z)
    : position_(glm::vec3(x, y, z)),
      rotation_(glm::vec3(0.0f, 0.0f, 0.0f)),
      scale_(glm::vec3(1.0f, 1.0f, 1.0f)),
      model_(glm::mat4(1.0f))     
    {
        updateVectors();
    }

    Transform::Transform(const glm::vec3& position) 
    : position_(glm::vec3(position.x, position.y, position.z)),
      rotation_(glm::vec3(0.0f, 0.0f, 0.0f)),
      scale_(glm::vec3(1.0f, 1.0f, 1.0f)),
      model_(glm::mat4(1.0f))    
    {
        updateVectors();
    }

    Transform::Transform(const Transform& other) 
        : position_(other.position_),
        rotation_(other.rotation_),
        scale_(other.scale_),
        model_(other.model_)
    {}

    Transform& Transform::operator=(const Transform& other) 
    {
        if (this != &other) {
            position_ = other.position_;
            rotation_ = other.rotation_;
            scale_ = other.scale_;
            model_ = other.model_;
        }
        return *this;
    }

    Transform::Transform(Transform&& other) noexcept : 
          position_(std::move(other.position_)),
          rotation_(std::move(other.rotation_)),
          scale_(std::move(other.scale_)),
          model_(std::move(other.model_))  {}
    
    Transform& Transform::operator=(Transform&& other) noexcept{
        if (this != &other) {
            position_ = std::move(other.position_);
            rotation_ = std::move(other.rotation_);
            scale_    = std::move(other.scale_);
            model_    = std::move(other.model_);
        }
        return *this;
    }


    void Transform::translate(float x, float y, float z) {
        position_ += glm::vec3(x, y, z);
        updateVectors();
    }

    void Transform::translate(const glm::vec3& delta) {
        position_ += delta;
        updateVectors();
    }

    void Transform::setPosition(float x, float y, float z) {
        position_ = glm::vec3(x, y, z);
        updateVectors();
    }

    void Transform::setPosition(const glm::vec3& value) {
        position_ = value;
        updateVectors();
    }

    void Transform::rotate(float x, float y, float z) {
        rotation_ += glm::vec3(x, y, z);
        updateVectors();
    }

    void Transform::rotate(const glm::vec3& delta) {
        rotation_ += delta;
        updateVectors();
    }

    void Transform::setRotation(float x, float y, float z) {
        rotation_ = glm::vec3(x, y, z);
        updateVectors();        
    }

    void Transform::setRotation(const glm::vec3& value) {
        rotation_ = value;
        updateVectors();        
    }

    void Transform::applyScale(float x, float y, float z) {
        scale_ *= glm::vec3(x, y, z);
        updateVectors();
    }

    void Transform::applyScale(const glm::vec3& delta) {
        scale_ *= delta;
        updateVectors();
    }

    void Transform::setScale(float x, float y, float z) {
        scale_ = glm::vec3(x, y, z);
        updateVectors();
    }

    void Transform::setScale(const glm::vec3& value) {
        scale_ = value;
        updateVectors();
    }

    const glm::mat4& Transform::getModel() const {
        return model_;
    }

    const glm::vec3& Transform::getPosition() const {
        return position_;
    }
    const glm::vec3& Transform::getRotation() const {
        return rotation_;
    }
    const glm::vec3& Transform::getScale() const {
        return scale_;
    }


}