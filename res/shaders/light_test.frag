#version 330 core


uniform vec3 toy_color;
uniform vec3 light_color;

out vec4 frag_color;

in vec3 frag_pos;
in vec3 normal;
in vec2 coord;

uniform sampler2D texture1;
uniform sampler2D texture2;

uniform vec3 light_pos;

float ambientStrength = 0.2f;

void main() {

    vec3 ambient = ambientStrength * light_color;

    vec3 norm = normalize(normal);
    vec3 light_dir = normalize(light_pos - frag_pos);

    float diff = max(dot(norm, light_dir), 0.0f);
    vec3 diffuse = diff * light_color;

    vec3 light_result = (ambient + diffuse) * toy_color;

    frag_color = mix(texture(texture1, coord), 
                     texture(texture2, coord),
                     0.5f) * vec4(light_result, 1.0f);
}