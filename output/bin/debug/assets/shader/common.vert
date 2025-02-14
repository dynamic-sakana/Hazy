#version 460 core
layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec2 a_TextCoord;
layout (location = 2) in vec3 a_Normal;

layout (location = 0) uniform mat4 u_model;
layout (location = 1) uniform mat4 u_viewProj;

layout (location = 0) out vec2 textCoord;
layout (location = 1) out vec3 normal;
layout (location = 2) out vec3 fragPos;

void main()
{
    gl_Position = u_viewProj * u_model * vec4(a_Position, 1.0f);
    textCoord = a_TextCoord;
    normal = a_Normal;
    fragPos = vec3(u_model * vec4(a_Position, 1.0f));
}