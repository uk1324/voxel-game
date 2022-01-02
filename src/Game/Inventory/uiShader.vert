#version 430 core

layout (location = 0) in vec2 vertexPos;
layout (location = 1) in vec2 textureCoordinate;

uniform vec2 scale;
uniform vec2 offset;
uniform vec2 textureCoordScale;

out vec2 textureCoord;

void main()
{
	gl_Position = vec4(vertexPos * scale + offset, 0, 1);
	textureCoord = textureCoordinate * textureCoordScale;	
}