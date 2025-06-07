#version 330 core

out vec4 f_color;

uniform sampler2D u_texture;

in vec2 v_coord;

void main()
{
    f_color = texture(u_texture, v_coord);
}