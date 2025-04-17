#version 330 core
out vec4 FragColor;

in vec2 aPosOut;

uniform vec2 Resolution;

vec3 hsv2rgb(vec3 c)
{
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

void main()
{

    vec2 uv = gl_FragCoord.xy / Resolution;

    vec2 center = vec2(0.5f);

    vec2 pos = 2.0f * (uv - center);
    
    float angle = atan(pos.y, pos.x);
    float radius = length(pos);

    angle += 3.14159265359f * 2.0f;

    angle = mod(angle, 6.28318530718f);

    float hue = angle / 6.28318530718f;
    float sat = radius + 1.0f;
    float val = 0.8f;

    vec3 RGB = hsv2rgb(vec3(hue, sat, val));

    FragColor = vec4(RGB, 1.0f);
}