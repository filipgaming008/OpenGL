#version 460 core
out vec4 FragColor;

uniform vec3 lightPos;
uniform float Time;
uniform vec4 objectColor;
uniform vec3 viewPos;

in vec3 normal;
in vec3 fragPos;

void main(){

    vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - fragPos);
    vec3 viewDir = normalize(viewPos - fragPos);

    // Ambient light
    vec3 ambient = 0.1f * lightColor;
    
    // Diffuse light
    float diff = max(dot(norm, lightDir), 0.0f);
    vec3 diffuse = diff * lightColor;

    // Specular light
    vec3 reflectDir = normalize(reflect(-lightDir, norm));
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 128);
    vec3 specular = 0.5f * spec * lightColor;

    vec3 result = (ambient + diffuse + specular) * objectColor.rgb;

    FragColor = vec4(result, objectColor.a);
}