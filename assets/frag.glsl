#version 460 core
layout (location = 0) in vec2 TextCoord;
layout (location = 0) out vec4 fragColor;
void main()
{
    fragColor = vec4(TextCoord, 1.0f, 1.0f);
}