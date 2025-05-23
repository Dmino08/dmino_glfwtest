#version 330 core

out vec4 frag_color;

struct Material {
    sampler2D diffuse;
    sampler2D specular;

    float shininess;
};

struct Light {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct DirectionalLight {
    Light base;

    vec3 direction;
};

struct Attenuation {
    float constant;
    float linear;
    float quadratic;
};

struct PointLight {
    Light base;

    Attenuation attenuation;
    vec3 position;
};

struct SpotLight {
    Light base;

    float cut_off;
    float outer_cut_off;

    Attenuation attenuation;
    vec3 position;
    vec3 direction;
};

in vec3 frag_pos;
in vec3 normal;
in vec2 coord;

#define POINT_LIGHT_SIZE 4

uniform vec3 view_pos;
uniform Material material;
uniform DirectionalLight direction_light;
uniform PointLight point_lights[POINT_LIGHT_SIZE];
uniform SpotLight spot_light; 
uniform bool on_flash_light;

uniform bool use_specular_map;

vec3 calculateDirectionalLight(vec3 texture_color) {
// ambient
    vec3 ambient = direction_light.base.ambient * texture_color;

// diffuse
    vec3 norm = normalize(normal);
    vec3 light_dir = normalize(-direction_light.direction);
    float diff = max(dot(light_dir, norm), 0.0);
    vec3 diffuse = direction_light.base.diffuse * diff * texture_color;

// specular
    vec3 view_dir = normalize(view_pos - frag_pos);
    vec3 reflect_dir = reflect(-light_dir, norm);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);
    vec3 spec_color = use_specular_map ? vec3(texture(material.specular, coord)) : vec3(0.0f);
    vec3 specular = direction_light.base.specular * spec * spec_color;

// result
    vec3 result = ambient + diffuse + specular;
    return result;
}

vec3 calculatePointLights(vec3 texture_color) {
// predeclaration 
    vec3 result;
    
    for(int i = 0; i < POINT_LIGHT_SIZE; i++) {
    // ambient
        vec3 ambient = point_lights[i].base.ambient * texture_color;
    // diffuse
        vec3 norm = normalize(normal);
        vec3 light_dir = normalize(point_lights[i].position - frag_pos);
        float diff = max(dot(light_dir, norm), 0.0);
        vec3 diffuse = point_lights[i].base.diffuse * diff * texture_color;
        // specular
        vec3 view_dir = normalize(view_pos - frag_pos);
        vec3 reflect_dir = reflect(-light_dir, norm);
        float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);
        vec3 spec_color = use_specular_map ? vec3(texture(material.specular, coord)) : vec3(0.0f);
        vec3 specular = point_lights[i].base.specular * spec * spec_color;
    // attenuation
        float distance = length(point_lights[i].position - frag_pos);
        float attenuation = 1.0 / (point_lights[i].attenuation.constant + 
                                   point_lights[i].attenuation.linear * distance + 
                                   point_lights[i].attenuation.quadratic * (distance * distance));
        
        diffuse *= attenuation;
        specular *= attenuation;

        result += ambient + diffuse + specular;
    }
// result
    return result;
}

vec3 calculateSpotLight(vec3 texture_color) {
// ambient
    vec3 ambient = spot_light.base.ambient * texture_color;
// diffuse
    vec3 norm = normalize(normal);
    vec3 light_dir = normalize(spot_light.position - frag_pos);
    float diff = max(dot(light_dir, norm), 0.0);
    vec3 diffuse = spot_light.base.diffuse * diff * texture_color;
    // specular
    vec3 view_dir = normalize(view_pos - frag_pos);
    vec3 reflect_dir = reflect(-light_dir, norm);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);
    vec3 spec_color = use_specular_map ? vec3(texture(material.specular, coord)) : vec3(0.0f);
    vec3 specular = spot_light.base.specular * spec * spec_color;
// spot light
    float theta = dot(light_dir, normalize(-spot_light.direction));
    float epsilon = spot_light.cut_off - spot_light.outer_cut_off;
    float intensity = clamp((theta - spot_light.outer_cut_off) / epsilon, 0.0, 1.0);

    diffuse *= intensity;
    specular *= intensity;
// attenuation
    float distance = length(spot_light.position - frag_pos);
    float attenuation = 1.0 / (spot_light.attenuation.constant + 
                            spot_light.attenuation.linear * distance + 
                            spot_light.attenuation.quadratic * (distance * distance));
        
    diffuse *= attenuation;
    specular *= attenuation;    
// result
    vec3 result = ambient + diffuse + specular;
    return result;
}


void main() {

    vec4 texture_rgba = texture(material.diffuse, coord);
    vec3 texture_color = vec3(texture_rgba);
    float alfa = texture_rgba.a;
    // if(alfa < 0.1)
    //     discard;

    vec3 dir = calculateDirectionalLight(texture_color);
    vec3 point = calculatePointLights(texture_color);

    vec3 spot= vec3(0.0);
    if (on_flash_light) {
        spot = calculateSpotLight(texture_color);
    }
// result
    vec3 result = dir + point + spot;
    frag_color = vec4(result, alfa);
}