#version 330 core

out vec4 f_color;

uniform sampler2D u_texture;

in vec2 coord;

void main()
{
    float depth_value = texture(u_texture, coord).r;
    f_color = vec4(vec3(depth_value), 1.0);
}