#version 330 core

out vec4 frag_color;
in vec2 coord;

uniform sampler2D texture0;

void main() {

   frag_color = texture2D(texture0, coord); 
}