#version 430 core

layout (location = 0) in vec3 vertexPos;
layout (location = 1) in vec2 textureCoordinate;
layout (location = 2) in float face;

uniform mat4 transform;

out vec2 textureCoord;
out flat uint faceIndex;

void main()
{
	gl_Position = transform * vec4(vertexPos, 1);
	textureCoord = textureCoordinate;
	faceIndex = uint(face);
}