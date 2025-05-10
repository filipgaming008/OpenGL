#version 460 core
out vec4 FragColor;

uniform sampler2D ourTexture;
uniform vec2 resolution;

in vec2 TexCoord;

void main(){
    vec2 uv = (gl_FragCoord.xy * 2.0f - resolution.xy) / resolution.y;
    vec4 result = vec4(1.0f);
    float radius = 0.95f;
    float dist = length(uv - vec2(0.0f));
    if (dist < radius) {
        if (dist < radius - 0.1f) {
            if(dist < radius - 0.89f){
                result = vec4(0.0f, 0.6f, 0.0f, 1.0f); // Green
                if (dist < radius - 0.92f) {
                    result = vec4(0.6f, 0.0f, 0.0f, 1.0f); // Red
                }
            }else{
                float angle = atan(uv.x, uv.y);
                float angleDeg = degrees(angle) + 7.5f;
                float angleMod = mod(angleDeg, 30.0f);
                if (angleMod < 15.0f) {
                    if(dist < radius - 0.15f && dist > radius - 0.5f || dist < radius - 0.55f && dist > radius - 0.9f){
                        result = vec4(0.0f, 0.0f, 0.0f, 1.0f); // Black
                    } else{
                        result = vec4(0.6f, 0.0f, 0.0f, 1.0f); // Red
                    }
                } else {
                    if(dist < radius - 0.15f && dist > radius - 0.5f || dist < radius - 0.55f && dist > radius - 0.9f){
                        result = vec4(1.0f, 1.0f, 1.0f, 1.0f); // White
                    } else{
                        result = vec4(0.0f, 0.6f, 0.0f, 1.0f); // Green
                    }
                }
            }
        }else{
            result = vec4(0.0f, 0.0f, 0.0f, 1.0f);
        }
    }else{
        result = vec4(0.0f, 0.0f, 0.0f, 0.0f);
    }

    FragColor = result;
}