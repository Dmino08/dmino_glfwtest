#pragma once

#include <glm/glm.hpp>

#define PROJECTION "u_projection"
#define VIEW "u_view"
#define MODEL "u_model"
#define PROJ_VIEW "u_proj_view"
#define PROJ_VIEW_MODEL "u_pvm"
#define PVM "u_pvm"
#define PROJ_MODEL "u_proj_model"

class Shader;

void setPVM(Shader& shader, const glm::mat4& pvm);
void setMatrices(Shader& shader, const glm::mat4& projmodel);
void setMatrices(Shader& shader, const glm::mat4& projview, const glm::mat4& model);
void setMatrices(Shader& shader, const glm::mat4& projection, const glm::mat4& view, const glm::mat4& model);


// NAMING RULES FOR GLSL SHADERS
/**
 * Attributes: a_variable_name
 * Vertex shader variables: v_variable_name
 * Fragment shader variables: f_variable_name
 * Geometry shader variables: g_variable_name
 * Uniform variables: u_variable_name
 * 
 */