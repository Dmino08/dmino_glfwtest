#version 330 core

out vec4 f_color;

in VS_OUT
{
    vec3 frag_pos;
    vec3 normal;
    vec2 coord; 
} fs_in;

uniform sampler2D u_diffuse;
uniform samplerCube u_shadow_map;

uniform vec3 u_light_pos;
uniform vec3 u_view_pos;

uniform float u_far_plane;


float shadowCalculation(vec3 frag_pos) 
{
    vec3 frag_to_light = frag_pos - u_light_pos;
    
    float closest_depth = texture(u_shadow_map, frag_to_light).r;
    closest_depth *= u_far_plane;

    float current_depth = length(frag_to_light);

    float shadow = 0.0;
    float bias = 0.05;
    float samples = 4.0;
    float offset = 0.1;

     for(float x =-offset; x < offset; x += offset / (samples * 0.5))
    {
        for(float y =-offset; y < offset; y += offset / (samples * 0.5))
        {
            for(float z =-offset; z < offset; z += offset / (samples * 0.5))
            {
                float closestDepth = texture(u_shadow_map, frag_to_light +
                vec3(x, y, z)).r;
                closestDepth *= u_far_plane; // undo mapping [0;1]
                if(current_depth - bias > closestDepth)
                {
                    shadow += 1.0;
                }
            }
        }
    }
    shadow /= (samples * samples * samples);
    
    return shadow;
}

void main()
{
        vec3 color = texture(u_diffuse, fs_in.coord).rgb;
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