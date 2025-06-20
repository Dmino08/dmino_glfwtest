#version 330 core

layout(location = 0) in vec3 a_pos;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec2 a_tex_coords;

out vec2 v_tex_coords;

uniform mat4 u_projection;
uniform mat4 u_view;
uniform mat4 u_model;

void main()
{
    v_tex_coords = a_tex_coords;
    gl_Position = u_projection * u_view * u_model * vec4(a_pos, 1.0);
}