#version 330 core

out vec4 frag_color;

in vec2 coord;

uniform sampler2D screen_texture;

void main() {
    vec2 resolution = vec2(1980, 1080);
    vec2 pixelSize = vec2(8, 8);
    vec2 blockCoord = floor(coord * resolution / pixelSize) * pixelSize;
    vec2 snappedUV = blockCoord / resolution;

    vec3 color = vec3(texture(screen_texture, snappedUV));
    float average = 0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b;
    frag_color = vec4(vec3(average), 1.0);
}