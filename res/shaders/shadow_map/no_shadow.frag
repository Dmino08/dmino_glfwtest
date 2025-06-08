#version 330 core

out vec4 f_color;

in VS_OUT
{
    vec3 frag_pos;
    vec3 normal;
    vec2 coord;
    vec4 frag_pos_light_space;    
} fs_in;

uniform sampler2D u_diffuse;
uniform sampler2D u_shadow_map;

uniform vec3 u_light_pos;
uniform vec3 u_view_pos;


void main()
{
    f_color = texture(u_diffuse, fs_in.coord);
}