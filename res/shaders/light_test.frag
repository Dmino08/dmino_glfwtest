#version 330 core

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

uniform Material material;

uniform vec3 toy_color;
uniform vec3 light_color;

out vec4 frag_color;

in vec3 frag_pos;
in vec3 normal;
in vec2 coord;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform vec3 light_pos;
uniform vec3 view_pos;

float ambient_strength = 0.2f;
float specular_strength = 0.5f;

void main() {

    vec3 ambient = light_color * material.ambient;


    vec3 norm = normalize(normal);
    vec3 light_dir = normalize(light_pos - frag_pos);
    float diff = max(dot(norm, light_dir), 0.0f);
    vec3 diffuse = light_color * (diff * material.diffuse);


    vec3 view_dir = normalize(view_pos - frag_pos);
    vec3 reflect_dir = reflect(-light_dir, norm);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);
    vec3 specular = light_color * (spec * material.specular);


    vec3 light_result = (ambient + diffuse + specular) * toy_color;
    vec4 texture_color = mix(texture(texture1, coord), 
                             texture(texture2, coord),
                             0.5f);
    frag_color = texture_color * vec4(light_result, 1.0f);
}