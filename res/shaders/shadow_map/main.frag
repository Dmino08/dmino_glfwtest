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

float shadowCalculation(vec4 frag_pos_light_space, vec3 normal, vec3 light_dir) 
{
    // perform perspective divide
    vec3 proj_coord = frag_pos_light_space.xyz / frag_pos_light_space.w;
     // transform to [0,1] range
    proj_coord = proj_coord * 0.5 + 0.5;
    // If z is further than frustrum far
    if(proj_coord.z > 1.0)
        return 0.0;
    // get closest depth value from light’s perspective (using
    // [0,1] range fragPosLight as coords)
    float closest_depth = texture(u_shadow_map, proj_coord.xy).r;
     // get depth of current fragment from light’s perspective
    float current_depth = proj_coord.z;
    // bias
    float bias = max(0.05 * (1.0- dot(normal, light_dir)), 0.005);

     // check whether current frag pos is in shadow
    float shadow = current_depth - bias > closest_depth ? 1.0 : 0.0;

    return shadow;
}

void main()
{
    vec3 color = texture(u_diffuse, fs_in.coord).rgb;
    vec3 normal = normalize(fs_in.normal);
    vec3 light_color = vec3(1.0);
    // ambient
    vec3 ambient = 0.15 * color;
    // diffuse
    vec3 light_dir = normalize(u_light_pos - fs_in.frag_pos);
    float diff = max(dot(light_dir, normal), 0.0);
    vec3 diffuse = diff * light_color;
    // specular
    vec3 view_dir = normalize(u_view_pos - fs_in.frag_pos);
    float spec = 0.0;
    vec3 halfway_dir = normalize(light_dir + view_dir);
    spec = pow(max(dot(normal, halfway_dir), 0.0), 64.0);
    vec3 specular = spec * light_color;
    // calculate shadow
    float shadow = shadowCalculation(fs_in.frag_pos_light_space, normal, light_dir);
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;
    // result
    f_color = vec4(lighting, 1.0);

}