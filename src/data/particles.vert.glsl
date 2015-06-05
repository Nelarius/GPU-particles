#version 430 core
in vec4 vert;
uniform mat4 modelViewMatrix;
out float intensity;

void main() {
    intensity = vert.w;
    gl_Position = modelViewMatrix * vec4(vert.xyz, 1.0);
}
