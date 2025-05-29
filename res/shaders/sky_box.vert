#version 330 core
layout (location = 0) in vec3 a_pos;

out vec3 coord;

uniform mat4 projection;
uniform mat4 view;

void main() {
    coord = a_pos;
    vec4 pos = projection * view * vec4(a_pos, 1.0);
    gl_Position = pos.xyww;
}