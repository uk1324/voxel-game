#version 430 core

layout (location = 0) in vec2 vertexPos;
layout (location = 1) in vec2 vertexTexturePos;

out vec2 texturePos;

void main()
{
	gl_Position = vec4(vertexPos, 0.0, 1.0);
	texturePos = vertexTexturePos;
}