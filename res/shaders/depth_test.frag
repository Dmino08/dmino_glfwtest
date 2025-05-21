#version 330 core

out vec4 frag_color;

float near = 0.1;
float far = 100.0;

float LinearizeDepth(float depth) {
    float z = depth * 2.0 - 1.0; // Back to NDC
    return (2.0 * near * far) / (far + near - z * (far - near));    
}

void main() {
    float linearDepth = LinearizeDepth(gl_FragCoord.z);
    linearDepth = linearDepth / far; // Normalize to [0, 1]
    frag_color = vec4(vec3(linearDepth), 1.0);
}