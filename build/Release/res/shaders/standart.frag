#version 330 core

out vec4 frag_color;
in vec2 texture_coord;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main() {

   frag_color = mix(texture2D(texture1, texture_coord), 
               texture2D(texture2, texture_coord * 2.0f), 0.2f);   
   
}