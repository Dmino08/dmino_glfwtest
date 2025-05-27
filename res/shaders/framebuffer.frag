#version 330 core

out vec4 frag_color;

in vec2 coord;

uniform sampler2D screen_texture;

void main() {
    vec3 color = vec3(texture(screen_texture, coord));
    float average = 0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b;
    frag_color = vec4(vec3(average), 1.0);
}