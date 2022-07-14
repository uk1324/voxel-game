#version 430 core
layout (location = 0) in vec3 vertexPos;

out vec3 texturePos;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    texturePos = vertexPos;
    vec4 pos = projection * view * vec4(vertexPos, 1.0);
    gl_Position = pos.xyww;
} 