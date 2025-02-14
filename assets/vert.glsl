#version 460 core
layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec2 a_TextCoord;
layout (location = 0) uniform mat4 u_model;
layout (location = 1) uniform mat4 u_viewProjection;
out vec2 TextCoord;
void main()
{
    gl_Position = u_viewProjection * u_model * vec4(a_Position, 1.0f);
    TextCoord = a_TextCoord;
}