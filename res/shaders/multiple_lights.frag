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

vec3 calculateDirectionalLight() {
// ambient
    vec3 ambient = direction_light.base.ambient * vec3(texture(material.diffuse, coord));

// diffuse
    vec3 norm = normalize(normal);
    vec3 light_dir = normalize(-direction_light.direction);
    float diff = max(dot(light_dir, norm), 0.0);
    vec3 diffuse = direction_light.base.diffuse * diff * vec3(texture(material.diffuse, coord));

// specular
    vec3 view_dir = normalize(view_pos - frag_pos);
    vec3 reflect_dir = reflect(-light_dir, norm);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);
    vec3 specular = direction_light.base.specular * spec * vec3(texture(material.specular, coord));

// result
    vec3 result = ambient + diffuse + specular;
    return result;
}

vec3 calculatePointLights() {
// predeclaration 
    vec3 result;
    
    for(int i = 0; i < POINT_LIGHT_SIZE; i++) {
    // ambient
        vec3 ambient = point_lights[i].base.ambient * vec3(texture(material.diffuse, coord));
    // diffuse
        vec3 norm = normalize(normal);
        vec3 light_dir = normalize(point_lights[i].position - frag_pos);
        float diff = max(dot(light_dir, norm), 0.0);
        vec3 diffuse = point_lights[i].base.diffuse * diff * vec3(texture(material.diffuse, coord));
        // specular
        vec3 view_dir = normalize(view_pos - frag_pos);
        vec3 reflect_dir = reflect(-light_dir, norm);
        float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);
        vec3 specular = point_lights[i].base.specular * spec * vec3(texture(material.specular, coord));
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

vec3 calculateSpotLight() {
// ambient
    vec3 ambient = spot_light.base.ambient * vec3(texture(material.diffuse, coord));
// diffuse
    vec3 norm = normalize(normal);
    vec3 light_dir = normalize(spot_light.position - frag_pos);
    float diff = max(dot(light_dir, norm), 0.0);
    vec3 diffuse = spot_light.base.diffuse * diff * vec3(texture(material.diffuse, coord));
    // specular
    vec3 view_dir = normalize(view_pos - frag_pos);
    vec3 reflect_dir = reflect(-light_dir, norm);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);
    vec3 specular = spot_light.base.specular * spec * vec3(texture(material.specular, coord));
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

    vec3 dir = calculateDirectionalLight();
    vec3 point = calculatePointLights();

    vec3 spot;
    if (on_flash_light) {
        spot = calculateSpotLight();
    }
// result
    vec3 result = dir + point + spot;
    frag_color = vec4(result, 1.0f);
}