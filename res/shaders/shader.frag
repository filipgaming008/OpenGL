#version 460 core
out vec4 FragColor;

uniform vec3 lightPos;
uniform float Time;

in vec3 normal;
in vec3 fragPos;

void main(){

    vec3 lightColor = vec3(1.0);
    vec3 objectColor = vec3(1.0, 0.5, 0.31);

    // Amvient light
    vec3 ambient = 0.1 * lightColor;
    // Diffuse light
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 result = (ambient + diffuse) * objectColor;

    // Specular light

    FragColor = vec4(result, 1.0);
}