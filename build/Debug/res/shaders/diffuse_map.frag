#version 330 core

out vec4 frag_color;

struct Material {
    sampler2D diffuse;
    sampler2D specular;

    float shininess;
};

struct Light {
    vec3 position;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 frag_pos;
in vec3 normal;
in vec2 coord;


uniform vec3 view_pos;
uniform Material material;
uniform Light light;


void main() {

    vec3 ambient = light.ambient * vec3(texture(material.diffuse, coord));


    vec3 norm = normalize(normal);
    vec3 light_dir = normalize(light.position - frag_pos);
    float diff = max(dot(norm, light_dir), 0.0f);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, coord));


    vec3 view_dir = normalize(view_pos - frag_pos);
    vec3 reflect_dir = reflect(-light_dir, norm);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0f), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, coord));


    vec3 result = ambient + diffuse + specular;
    frag_color = vec4(result, 1.0f);
}