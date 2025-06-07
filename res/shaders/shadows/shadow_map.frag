#version 330 core

out vec4 frag_color;

in vec3 frag_pos;
in vec3 normal;
in vec2 coord;

uniform sampler2D f_texture;


void main() {

    frag_color = texture(f_texture, coord);
}