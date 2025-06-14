#version 330 core

out vec4 f_color;

in VS_OUT
{
    vec3 frag_pos;
    vec3 normal;
    vec2 tex_coords; 
} fs_in;

uniform sampler2D u_diffuse;
uniform samplerCube u_shadow_map;

uniform vec3 u_light_pos;
uniform vec3 u_view_pos;

uniform float u_far_plane;

vec3 sampleOffsetDirections[20] = vec3[]
 (
 vec3( 1, 1, 1), vec3( 1,-1, 1), vec3(-1,-1, 1), vec3(-1, 1, 1),
 vec3( 1, 1,-1), vec3( 1,-1,-1), vec3(-1,-1,-1), vec3(-1, 1,-1),
 vec3( 1, 1, 0), vec3( 1,-1, 0), vec3(-1,-1, 0), vec3(-1, 1, 0),
 vec3( 1, 0, 1), vec3(-1, 0, 1), vec3( 1, 0,-1), vec3(-1, 0,-1),
 vec3( 0, 1, 1), vec3( 0,-1, 1), vec3( 0,-1,-1), vec3( 0, 1,-1)
 );

float shadowCalculation(vec3 frag_pos) 
{
    vec3 frag_to_light = frag_pos - u_light_pos;
    
    float closest_depth = texture(u_shadow_map, frag_to_light).r;
    closest_depth *= u_far_plane;

    float current_depth = length(frag_to_light);

    float shadow = 0.0;
    float bias = 0.15;
    int samples = 20;
    float view_dis = length(u_view_pos- frag_pos);
    float disk_rad = (1.0 + (view_dis / u_far_plane)) / 25.0;
    for(int i = 0; i < samples; ++i)
    {
        float closestDepth = texture(u_shadow_map, frag_to_light +
        sampleOffsetDirections[i] * disk_rad).r;
        closestDepth *= u_far_plane; // undo mapping [0;1]
        if(current_depth - bias > closestDepth)
            shadow += 1.0;
    }
    shadow /= float(samples);
    return shadow;
}

void main()
{
        vec3 color = texture(u_diffuse, fs_in.tex_coords).rgb;
        vec3 normal = normalize(fs_in.normal);
        vec3 light_color = vec3(1.0);
        // ambient
        vec3 ambient = 0.4 * color;
        // diffuse
        vec3 light_dir = normalize( u_light_pos - fs_in.frag_pos);
        float diff = max(dot(light_dir, normal), 0.0);
        vec3 diffuse = diff * light_color;
        // specular
        vec3 view_dir = normalize(u_view_pos - fs_in.frag_pos);
        float spec = 0.0;
        vec3 halfway_dir = normalize(light_dir + view_dir);
        spec = pow(max(dot(normal, halfway_dir), 0.0), 64.0);
        vec3 specular = spec * light_color;
        // calculate shadow
        float shadow = shadowCalculation(fs_in.frag_pos);
        vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;
        // result
        f_color = vec4(lighting, 1.0);
}