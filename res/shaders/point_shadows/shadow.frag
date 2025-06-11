#version 330 core

uniform vec3 u_light_pos;
uniform float u_far_plane;

in vec4 g_frag_pos;

void main()
{
    float light_distance = length(g_frag_pos.xyz - u_light_pos);

    light_distance = light_distance / u_far_plane;

    gl_FragDepth = light_distance;
}