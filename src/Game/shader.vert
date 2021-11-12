#version 430 core

layout (location = 0) in vec2 vertex;

uniform vec2 offset;

void main()
{
	gl_Position = vec4(vertex + offset, 0.0, 1.0);
}