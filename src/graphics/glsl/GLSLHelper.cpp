#include "graphics/glsl/GLSLHelper.hpp"
#include "graphics/core/Shader.hpp"

void setPVM(Shader& shader, const glm::mat4& pvm) 
{
    shader.uniformMat4(PVM, pvm);
}
void setMatrices(Shader& shader, const glm::mat4& projmodel) 
{
    shader.uniformMat4(PROJ_MODEL, projmodel);
}
void setMatrices(Shader& shader, const glm::mat4& projview, const glm::mat4& model) 
{
    shader.uniformMat4(PROJ_VIEW, projview);
    shader.uniformMat4(MODEL, model);
}
void setMatrices(Shader& shader, const glm::mat4& projection, const glm::mat4& view, const glm::mat4& model) 
{
    shader.uniformMat4(PROJECTION, projection);
    shader.uniformMat4(VIEW, view);
    shader.uniformMat4(MODEL, model);
}