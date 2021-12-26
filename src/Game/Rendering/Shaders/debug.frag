#version 430 core

uniform vec3 color;

in vec2 texCoord;

out vec4 outColor;

void main()
{
	outColor = vec4(color, 1);
}