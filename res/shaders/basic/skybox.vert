#version 330 core
layout (location = 0) in vec3 a_pos;

out vec3 v_tex_coords;

uniform mat4 u_projview;

void main()
{
    v_tex_coords = a_pos;
    vec4 pos = u_projview * vec4(a_pos, 1.0);
    gl_Position = vec4(pos.xy, pos.w, pos.w);
}