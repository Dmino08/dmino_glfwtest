#version 330 core

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float shininess;
};

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};


out vec4 frag_color;

in vec3 frag_pos;
in vec3 normal;
in vec2 coord;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform vec3 view_pos;

uniform Material material;
uniform Light light;


void main() {

    vec3 ambient = light.ambient * material.ambient;


    vec3 norm = normalize(normal);
    vec3 light_dir = normalize(light.position - frag_pos);
    float diff = max(dot(norm, light_dir), 0.0f);
    vec3 diffuse = light.diffuse * (diff * material.diffuse);


    vec3 view_dir = normalize(view_pos - frag_pos);
    vec3 reflect_dir = reflect(-light_dir, norm);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);


    vec3 light_result = ambient + diffuse + specular;
    vec4 texture_color = mix(texture(texture1, coord), 
                             texture(texture2, coord),
                             0.5f);
    frag_color = texture_color * vec4(light_result, 1.0f);
}