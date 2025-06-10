#pragma once

#include <glm/glm.hpp>

// MATRIX VARIABLES
constexpr const char* PROJECTION = "u_projection";
constexpr const char* VIEW = "u_view";
constexpr const char* MODEL = "u_model";
constexpr const char* PROJ_VIEW = "u_projview";
constexpr const char* PROJ_VIEW_MODEL = "u_pvm";
constexpr const char* PVM = "u_pvm";
constexpr const char* PROJ_MODEL = "u_proj_model";
constexpr const char* NORMAL = "u_normal";

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
constexpr unsigned int SHADOW_C_1   = 128;                      // 128
constexpr unsigned int SHADOW_K_01  = SHADOW_C_1;               // 128
constexpr unsigned int SHADOW_C_2   = 2 * SHADOW_C_1;           // 256
constexpr unsigned int SHADOW_K_02  = SHADOW_C_2;               // 256
constexpr unsigned int SHADOW_C_5   = 2 * SHADOW_C_2;           // 512
constexpr unsigned int SHADOW_K_05  = SHADOW_C_5;               // 512
constexpr unsigned int SHADOW_K_1   = 2 * SHADOW_C_5;           // 1024
constexpr unsigned int SHADOW_K_2   = 2 * SHADOW_K_1;           // 2048
constexpr unsigned int SHADOW_K_3   = SHADOW_K_1 + SHADOW_K_2;  // 3072
constexpr unsigned int SHADOW_K_4   = 2 * SHADOW_K_2;           // 4096
constexpr unsigned int SHADOW_K_5   = SHADOW_K_1 + SHADOW_K_4;  // 5120
constexpr unsigned int SHADOW_K_6   = 2 * SHADOW_K_3;           // 6144
constexpr unsigned int SHADOW_K_7   = SHADOW_K_1 + SHADOW_K_6;  // 7168
constexpr unsigned int SHADOW_K_8   = 2 * SHADOW_K_4;           // 8192
constexpr unsigned int SHADOW_K_9   = 3 * SHADOW_K_3;           // 9216
constexpr unsigned int SHADOW_K_10  = 2 * SHADOW_K_5;           // 10240
constexpr unsigned int SHADOW_K_16  = 2 * SHADOW_K_8;           // 16384


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