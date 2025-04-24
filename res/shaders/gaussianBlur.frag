#version 460 core

uniform sampler2D u_source;
uniform bool u_horizontal;
uniform vec2 u_resolution;

in vec2 texCoords;
out vec4 FragColor;

void main() {
    // σ=4 kernel weights (normalized)
    float weights[5] = float[] (0.152073, 0.222381, 0.251093, 0.222381, 0.152073);
    float kernel[9] = float[] (
        weights[4], weights[3], weights[2], weights[1], weights[0], 
        weights[1], weights[2], weights[3], weights[4]
    );
    
    vec2 direction = u_horizontal ? vec2(1.0, 0.0) : vec2(0.0, 1.0);
    vec2 texel = 1.0 / u_resolution;
    
    // Center pixel (index 4 in kernel)
    vec3 color = texture(u_source, texCoords).rgb * kernel[4];
    
    // Left/bottom samples (indices 0-3)
    for (int i = 0; i < 4; i++) {
        color += texture(u_source, texCoords - direction * texel * float(4-i)).rgb * kernel[i];
    }
    
    // Right/top samples (indices 5-8) 
    for (int i = 5; i < 9; i++) {
        color += texture(u_source, texCoords + direction * texel * float(i-4)).rgb * kernel[i];
    }
    
    FragColor = vec4(color, 1.0);
}= vec4(result, 1.0);
}