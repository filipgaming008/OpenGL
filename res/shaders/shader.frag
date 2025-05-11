#version 460 core
out vec4 FragColor;

uniform vec3 lightPos;
uniform float Time;
uniform sampler2D ourTexture;

in vec3 normal;
in vec3 fragPos;
in vec2 TexCoord;

void main(){

    vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
    vec3 objectColor = vec3(1.0f, 0.5f, 0.31f);

    // Ambient light
    vec3 ambient = 0.1f * lightColor;
    
    // Diffuse light
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(norm, lightDir), 0.0f);
    vec3 diffuse = diff * lightColor;

    vec3 result = (ambient + diffuse) * objectColor;

    // Specular light

    FragColor = texture(ourTexture, TexCoord * 4.0f) * vec4(result, 1.0f);
}