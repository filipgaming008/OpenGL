#version 460 core

out vec4 FragColor;
uniform sampler2D scene;
in vec2 texCoords;

void main() {
    FragColor = texture(scene, texCoords);
}