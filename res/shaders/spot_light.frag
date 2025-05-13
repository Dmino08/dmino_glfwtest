#version 330 core

out vec4 frag_color;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float cut_off;

    float constant;
    float linear;
    float quadratic;
};

in vec3 frag_pos;
in vec3 normal;
in vec2 coord;

uniform vec3 view_pos;
uniform Material material;
uniform Light light;

void main() {
    vec3 light_dir = normalize(light.position - frag_pos);
    float theta = dot(light_dir, normalize(-light.direction));
    vec3 result;

    vec3 ambient = light.ambient * vec3(texture(material.diffuse, coord));

    if (theta > light.cut_off) {

        vec3 norm = normalize(normal);
        float diff = max(dot(norm, light_dir), 0.0);
        vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, coord));

        vec3 view_dir = normalize(view_pos - frag_pos);
        vec3 reflect_dir = reflect(-light_dir, norm);
        float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);
        vec3 specular = light.specular * spec * vec3(texture(material.specular, coord));

        float distance = length(light.position - frag_pos); 
        float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

        diffuse *= attenuation;
        specular *= attenuation;

        result = ambient + diffuse + specular;

    } else {

        result = ambient;
    }

    frag_color = vec4(result, 1.0);
}