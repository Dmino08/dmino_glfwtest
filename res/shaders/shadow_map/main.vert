#version 330 core
layout (location = 0) in vec3 a_pos;
layout (location = 1 ) in vec3 a_normal;
layout (location = 2) in vec2 a_coord;
layout (location = 3) in vec3 a_offset;


out VS_OUT 
{
    vec3 frag_pos;
    vec3 normal;
    vec2 coord;
    vec4 frag_pos_light_space;
} vs_out;

uniform mat4 u_projection;
uniform mat4 u_view;
uniform mat4 u_model;
uniform mat4 u_light_space;

void main()
{
    vec3 pos = a_pos + a_offset;
    vs_out.frag_pos = vec3(u_model * vec4(pos, 1.0));
    vs_out.normal = transpose(inverse(mat3(u_model))) * a_normal;
    vs_out.coord = a_coord;
    vs_out.frag_pos_light_space = u_light_space * vec4(vs_out.frag_pos, 1.0);

    gl_Position = u_projection * u_view * u_model * vec4(pos, 1.0);
}