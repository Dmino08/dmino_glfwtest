#version 330 core
layout (location = 0) in vec3 a_pos;

layout (std140) uniform Matrices 
{
    uniform mat4 projection; // 16 // 0
    uniform mat4 view;  // 16 // 16
};

// uniform mat4 projection; // 16 // 0
// uniform mat4 view;  // 16 // 16
uniform mat4 model;

void main() {
    gl_Position = projection * view * model * vec4(a_pos, 1.0);
    float size = 250.0;
    gl_PointSize = (1.0 * size) / (gl_Position.z);
}