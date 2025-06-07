#include "graphics/glsl/GLSLHelper.hpp"
#include "graphics/core/Shader.hpp"

void setPVM(Shader& shader, const glm::mat4& pvm) 
{
    shader.uniformMatrix(PVM, pvm);
}
void setMatrices(Shader& shader, const glm::mat4& projmodel) 
{
    shader.uniformMatrix(PROJ_MODEL, projmodel);
}
void setMatrices(Shader& shader, const glm::mat4& projview, const glm::mat4& model) 
{
    shader.uniformMatrix(PROJ_VIEW, projview);
    shader.uniformMatrix(MODEL, model);
}
void setMatrices(Shader& shader, const glm::mat4& projection, const glm::mat4& view, const glm::mat4& model) 
{
    shader.uniformMatrix(PROJECTION, projection);
    shader.uniformMatrix(VIEW, view);
    shader.uniformMatrix(MODEL, model);
}