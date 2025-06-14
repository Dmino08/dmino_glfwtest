#version 330 core
layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_tex_coords;
// layout (location = 3) in vec3 a_offset;

out vec2 v_tex_coords;

uniform mat4 u_projview;
uniform mat4 u_model;

void main()
{  
    v_tex_coords = a_tex_coords;
    vec4 pos = vec4(a_pos, 1.0);
    gl_Position = u_projview * u_model * pos;
}