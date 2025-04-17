#version 330 core
out vec4 FragColor;

in vec2 aPosOut;

uniform vec3 Color1;
uniform vec3 Color2;
uniform vec2 Resolution;

void main()
{
    float mixval = aPosOut.x / Resolution.x;

    vec3 ourColor = mix(Color1, Color2, mixval);
    FragColor = vec4(ourColor, 1.0f);
}