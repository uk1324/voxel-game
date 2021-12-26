#version 430 core

layout (location = 0) in vec2 vertexPos;

out vec2 texCoord;

uniform vec2 viewportSize;

void main()
{
	gl_Position = vec4(vertexPos / viewportSize * (viewportSize.x * 0.03), 0, 1);
	texCoord = vertexPos;	
}