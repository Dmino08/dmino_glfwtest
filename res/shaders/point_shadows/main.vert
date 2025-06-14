#version 330 core
layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_tex_coords;
layout (location = 3) in vec3 a_offset;


out VS_OUT 
{
    vec3 frag_pos;
    vec3 normal;
    vec2 tex_coords;
} vs_out;

uniform mat4 u_projview;
uniform mat4 u_model;
uniform mat3 u_normal;

void main()
{
    vec4 world_pos = u_model * vec4(a_pos + a_offset, 1.0);
    vs_out.frag_pos = world_pos.xyz;
    vs_out.normal = u_normal * a_normal; // transpose(inverse(mat3(u_model))) * a_normal;
    vs_out.tex_coords = a_tex_coords;

    gl_Position = u_projview * world_pos;
}