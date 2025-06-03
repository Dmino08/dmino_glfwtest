#pragma once

#include "glm/gtc/matrix_transform.hpp"

namespace util {


    class Transform {
        glm::vec3 position_;
        glm::vec3 rotation_;
        glm::vec3 scale_;

        glm::mat4 model_;

        void updateVectors();

        public:
            Transform();
            Transform(glm::mat4 matrix);
            Transform(float x, float y, float z);
            Transform(const glm::vec3& position);

            Transform(Transform&& other) noexcept;
            Transform& operator=(Transform&& other) noexcept;

        // Position changing
            void translate(float x, float y, float z);
            void translate(const glm::vec3& delta);

            void setPosition(float x, float y, float z);
            void setPosition(const glm::vec3& value);
        // Rotation changing
            void rotate(float x, float y, float z);
            void rotate(const glm::vec3& delta);

            void setRotation(float x, float y, float z);
            void setRotation(const glm::vec3& value);
        // Scale changing
            void applyScale(float x, float y, float z);
            void applyScale(const glm::vec3& delta);

            void setScale(float x, float y, float z);
            void setScale(const glm::vec3& value);

            const glm::mat4& getModel() const;

            const glm::vec3& getPosition() const;
            const glm::vec3& getRotation() const;
            const glm::vec3& getScale() const;
    };

}