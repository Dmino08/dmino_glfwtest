#pragma once

#include <glm/glm.hpp>
#include <string>

class Shader;

namespace glsl {

    // ambient, diffuse, shininess
    struct Material {
        
        int diffuse;
        int specular;

        float shininess;

    };

    void setMaterial(Shader& shader, const std::string& name, const Material& material);

    // ambient, diffuse, specular
    struct Light {
        
        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular; 

    };

    // base (ambient, diffuse, specular),  direction
    struct DirectionalLight {
        
        Light base;

        glm::vec3 direction;

    };

    void setDirectionalLight(Shader& shader, const std::string& name, const DirectionalLight& light);

    struct Attenuation {

        float constant;
        float linear;
        float quadratic;

    };

    // base (ambient, diffuse, specular) attenuation (constant, linear, quadratic),  position
    struct PointLight {
        Light base;

        Attenuation attenuation;

        glm::vec3 position;
    };

    void setPointLight(Shader& shader, const std::string& name, const PointLight& light);

    // base (ambient, diffuse, specular) cut_off outer_cut_off attenuation (constant, linear, quadratic),  position
    struct SpotLight {
        Light base;

        float cut_off;
        float outer_cut_off;

        Attenuation attenuation;

        glm::vec3 position;
        glm::vec3 direction;
    };

    void setSpotLight(Shader& shader, const std::string& name, const SpotLight& light);

}