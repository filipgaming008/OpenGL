#version 460 core
out vec4 FragColor;

uniform sampler2D texture1;
uniform sampler2D texture2;

uniform float time;

in vec2 TexCoord;

void main(){

    FragColor = mix(texture(texture1, TexCoord * 10.0f), texture(texture2, TexCoord * 10.0f), sin(time) * 0.5f + 0.5f);
}