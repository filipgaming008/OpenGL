#version 460 core
uniform sampler2D hdrBuffer;
uniform sampler2D bloomBuffer;

in vec2 texCoords;
out vec4 FragColor;

void main() {
    vec3 hdrColor = texture(hdrBuffer, texCoords).rgb;
    vec3 bloomColor = texture(bloomBuffer, texCoords).rgb;
    
    // Combine with adjustable bloom strength
    vec3 result = hdrColor + bloomColor * 10.0;

    // Reinhard tone mapping
    result = result / (result + vec3(1.0));

    // // Gamma correction
    // result = pow(result, vec3(1.0 / 2.2));

    FragColor = vec4(result, 1.0);
}