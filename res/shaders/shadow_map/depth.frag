#version 330 core

out vec4 f_color;

uniform sampler2D u_texture;
uniform float u_near_plane;
uniform float u_far_plane;

in vec2 coord;

float LinearizeDepth(float depth)
 {
    float z = depth * 2.0- 1.0; // Back to NDC
    return (2.0 * u_near_plane * u_far_plane) / (u_far_plane + u_near_plane -
        z * (u_far_plane - u_near_plane));
 }

void main()
{
    float depth_value = texture(u_texture, coord).r;
    // f_color = vec4(vec3(LinearizeDepth(depth_value) / u_far_plane), 1.0);
    f_color = vec4(vec3(depth_value), 1.0);   // Orthographic
}