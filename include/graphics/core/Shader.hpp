#pragma once


#include <glad/glad.h>

#include <string>
#include <iostream>
#include <unordered_map>

#include <glm/glm.hpp>


#include <memory>




class Shader {
    GLuint ID_ = 0;
    
    std::unordered_map<std::string, unsigned int> uniformLocations_;

    unsigned int getUniformLocation(const std::string& name);
    
    public:
        Shader(GLuint id);
        ~Shader();

        void use() const;


        void uniform1i(const std::string& name, int x);
        void uniform2i(const std::string& name, int x, int y);
        void uniform2i(const std::string& name, glm::ivec2 xy);
        void uniform3i(const std::string& name, int x, int y, int z);
        void uniform3i(const std::string& name, glm::ivec3 xyz);
        void uniform1f(const std::string& name, float x);
        void uniform2f(const std::string& name, float x, float y);
        void uniform2f(const std::string& name, glm::vec2 xy);
        void uniform3f(const std::string& name, float x, float y, float z);
        void uniform3f(const std::string& name, glm::vec3 xyz);
        void uniformMatrix(const std::string& name, glm::mat4 matrix);

        void setMatrices(const glm::mat4& projection, const glm::mat4& view);


        static std::unique_ptr<Shader> create(const char* vertex_path, const char* fragment_path);
};
