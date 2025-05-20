#version 330 core

out vec4 frag_color;

in vec3 color;
in vec2 coord;

uniform sampler2D texture0;


void main() {
    frag_color = texture(texture0, coord) * vec4(color, 1.0f);
}