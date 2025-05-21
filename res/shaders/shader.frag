#version 460 core
out vec4 FragColor;

uniform vec3 viewPos;

in vec3 normal;
in vec3 fragPos;

struct Light {
    vec3 pos;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light;

struct Material {
    vec4 ambient;
    vec3 diffuse;
    vec3 specular;
    float shine;
};

uniform Material material;

void main(){
    vec3 lightDir = normalize(light.pos - fragPos);
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 norm = normalize(normal);

    // Ambient light
    vec3 ambient = light.ambient * material.ambient.rgb;
    
    // Diffuse light
    float diff = max(dot(norm, lightDir), 0.0f);
    vec3 diffuse = light.diffuse * (diff * material.diffuse);

    // Specular light
    vec3 reflectDir = normalize(reflect(-lightDir, norm));
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shine);
    vec3 specular = light.specular * (spec * material.specular);

    vec3 result = ambient + diffuse + specular;

    FragColor = vec4(result, material.ambient.a);
}