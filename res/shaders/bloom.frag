#version 460 core
uniform sampler2D scene;
in vec2 texCoords;
out vec4 FragColor;

void main() {
    vec3 color = texture(scene, texCoords).rgb;
    float brightness = dot(color, vec3(0.2126, 0.7152, 0.0722));
    if (brightness < 1.0){
     color = vec3(0.0);
    }
    FragColor = vec4(color, 1.0);
}