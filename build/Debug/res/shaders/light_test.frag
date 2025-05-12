#version 330 core


uniform vec3 toy_color;
uniform vec3 light_color;

out vec4 frag_color;

in vec2 coord;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main() {
    frag_color = mix(texture2D(texture1, coord), 
                     texture2D(texture2, coord),
                     0.5f) * vec4(toy_color, 1.0f) * vec4(light_color, 1.0f);
}