#version 330 core

out vec4 frag_color;

in vec3 pos;
in vec3 normal;
// in vec2 coord;

// uniform sampler2D tex;
uniform vec3 camera_pos;
uniform samplerCube skybox;

void main() {
    // frag_color = texture(tex, coord);
    float ratio = 1.0 / 1.52;
    vec3 I = normalize(pos - camera_pos);
   // vec3 R = reflect(I, normalize(normal));
    vec3 R = refract(I, normalize(normal), ratio);

    frag_color = vec4(texture(skybox, R).rgb, 1.0);
}