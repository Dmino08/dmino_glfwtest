#version 330 core
layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_coord;



uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec3 frag_pos;
out vec3 normal;
out vec2 coord;


void main() {

    gl_Position = projection * view * model * vec4(a_pos, 1.0f);
    
    frag_pos = vec3(model * vec4(a_pos, 1.0f));
    normal = a_normal;
    coord = a_coord;
}