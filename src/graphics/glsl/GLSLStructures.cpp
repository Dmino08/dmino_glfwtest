#include "graphics/glsl/GLSLStructures.hpp"

#include "graphics/core/Shader.hpp"

namespace glsl {

    void setMaterial(Shader& shader, const std::string& name, const Material& material) {
        shader.uniform1i(name + ".diffuse", material.diffuse);
        shader.uniform1i(name + ".specular", material.specular);

        shader.uniform1f(name + ".shininess", material.shininess);
    }

    void setDirectionalLight(Shader& shader, const std::string& name, const DirectionalLight& light) {
        shader.uniform3f(name + ".base.ambient", light.base.ambient);
        shader.uniform3f(name + ".base.diffuse", light.base.diffuse);
        shader.uniform3f(name + ".base.specular", light.base.specular);

        shader.uniform3f(name + ".direction", light.direction);
    }

    void setPointLight(Shader& shader, const std::string& name, const PointLight& light) {
        shader.uniform3f(name + ".base.ambient", light.base.ambient);
        shader.uniform3f(name + ".base.diffuse", light.base.diffuse);
        shader.uniform3f(name + ".base.specular", light.base.specular);

        shader.uniform1f(name + ".attenuation.constant", light.attenuation.constant);
        shader.uniform1f(name + ".attenuation.linear", light.attenuation.linear);
        shader.uniform1f(name + ".attenuation.quadratic", light.attenuation.quadratic);

        shader.uniform3f(name + ".position", light.position);
    }

    void setSpotLight(Shader& shader, const std::string& name, const SpotLight& light) {
        shader.uniform3f(name + ".base.ambient", light.base.ambient);
        shader.uniform3f(name + ".base.diffuse", light.base.diffuse);
        shader.uniform3f(name + ".base.specular", light.base.specular);

        shader.uniform1f(name + ".cut_off", glm::cos(glm::radians(light.cut_off)));
        shader.uniform1f(name + ".outer_cut_off", glm::cos(glm::radians(light.outer_cut_off)));

        shader.uniform1f(name + ".attenuation.constant", light.attenuation.constant);
        shader.uniform1f(name + ".attenuation.linear", light.attenuation.linear);
        shader.uniform1f(name + ".attenuation.quadratic", light.attenuation.quadratic);

        shader.uniform3f(name + ".position", light.position);
    }

}