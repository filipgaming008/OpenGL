#version 330 core
layout (location = 0) in vec2 aPos;

out vec3 ourColor;

uniform float R;
uniform float G;
uniform float B;

void main()
{
        gl_Position = vec4(aPos.x, aPos.y, 0.0f, 1.0f);
        ourColor = vec3(R, G, B);
}