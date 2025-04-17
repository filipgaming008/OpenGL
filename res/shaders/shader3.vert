#version 330 core
layout (location = 0) in vec2 aPos;

out vec2 aPosOut;

uniform vec2 Resolution;

void main()
{
    aPosOut = (aPos * 0.5f + 0.5f) * Resolution;

    gl_Position = vec4(aPos.x, aPos.y, 0.0f, 1.0f);
}