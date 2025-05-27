#version 330 core
layout (location = 0) in vec2 a_pos;
layout (location = 2) in vec2 a_coord;

out vec2 coord;

void main() {
    coord = a_coord;
    gl_Position = vec4(a_pos.x, a_pos.y, 0.0f, 1.0f);
}