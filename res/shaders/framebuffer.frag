#version 330 core

out vec4 frag_color;

in vec2 coord;

uniform sampler2D screen_texture;
// const float offset = 1.0 / 300.0;

void main() {
    // PIXEL PERFECT
    // vec2 resolution = vec2(1980, 1080);
    // vec2 pixelSize = vec2(8, 8);
    // vec2 blockCoord = floor(coord * resolution / pixelSize) * pixelSize;
    // vec2 snappedUV = blockCoord / resolution;

    // KERNEL EFFECT
    // vec2 offsets[9] = vec2[](
    // vec2(-offset, offset), // top-left
    // vec2( 0.0f, offset), // top-center
    // vec2( offset, offset), // top-right
    // vec2(-offset, 0.0f), // center-left
    // vec2( 0.0f, 0.0f), // center-center
    // vec2( offset, 0.0f), // center-right
    // vec2(-offset, -offset), // bottom-left
    // vec2( 0.0f, -offset), // bottom-center
    // vec2( offset, -offset) // bottom-right
    // );
    // Kernel
    // float kernel[9] = float[](
    // -1, -1, -1,
    // -1, 9, -1,
    // -1, -1, -1
    // );
    // Blur
    // float kernel[9] = float[](
    // 1.0 / 16, 2.0 / 16, 1.0 / 16,
    // 2.0 / 16, 4.0 / 16, 2.0 / 16,
    // 1.0 / 16, 2.0 / 16, 1.0 / 16
    // );
    // My kernel
    // float blur = 20.0;
    // float kernel[9] = float[](
    // -2.0, -1.0, 0.0, 
    // -1.0, 1.0, 1.0,
    // 0.0, 1.0, 2.0
    // );

    // vec3 sampleTex[9];
    // for(int i = 0; i < 9; i++)
    // {
    // sampleTex[i] = vec3(texture(screen_texture, coord.st +
    // offsets[i]));
    // }
    // vec3 col = vec3(0.0);
    // for(int i = 0; i < 9; i++)
    // col += sampleTex[i] * kernel[i];
    // frag_color = vec4(col, 1.0);

    // BLACK AND WHITE
    // vec3 color = vec3(texture(screen_texture, coord));
    // float average = 0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b;
    // frag_color = vec4(vec3(average), 1.0);

    frag_color = texture(screen_texture, coord);
}