#version 330 core
layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_normal;
// layout (location = 2) in vec2 a_coord;

out vec3 pos;
out vec3 normal;
// out vec2 coord;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main() {
    pos = vec3(model * vec4(a_pos,1.0));
    normal = mat3(transpose(inverse(model))) * a_normal;
    // coord = a_coord;
    gl_Position = projection * view * vec4(pos,1.0);
}