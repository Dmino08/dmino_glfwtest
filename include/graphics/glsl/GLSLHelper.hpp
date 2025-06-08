#pragma once

#include <glm/glm.hpp>

// MATRIX VARIABLES
constexpr const char* PROJECTION = "u_projection";
constexpr const char* VIEW = "u_view";
constexpr const char* MODEL = "u_model";
constexpr const char* PROJ_VIEW = "u_proj_view";
constexpr const char* PROJ_VIEW_MODEL = "u_pvm";
constexpr const char* PVM = "u_pvm";
constexpr const char* PROJ_MODEL = "u_proj_model";

// SOME STUFF
constexpr const char* VIEW_POS = "u_view_pos";
constexpr const char* LIGHT_POS = "u_light_pos";
constexpr const char* LIGHT_DIR = "u_light_dir";
constexpr const char* TEXTURE = "u_texture";
constexpr const char* DIFFUSE = "u_diffuse";
constexpr const char* SHADOW_MAP = "u_shadow_map";
constexpr const char* NEAR_PLANE = "u_near_plane";
constexpr const char* FAR_PLANE = "u_far_plane";

// SHADOW CONSTS
constexpr unsigned int SHADOW_C_5 = 512;
constexpr unsigned int SHADOW_K_1 = 1024;
constexpr unsigned int SHADOW_K_2 = 2048;
constexpr unsigned int SHADOW_K_4 = 4096;
constexpr unsigned int SHADOW_K_8 = 8192;
constexpr unsigned int SHADOW_K_16 = 16384;


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