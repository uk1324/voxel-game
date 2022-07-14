#version 430 core

vec3 faceIndexToNormal[] = {
 	vec3(0, 0, -1), // Front
 	vec3(0, 0, 1), // Back
 	vec3(-1, 0, 0), // Right
	vec3(1, 0, 0), // Left
 	vec3(0, -1, 0), // Bottom
 	vec3(0, 1, 0) // Top
};

layout (location = 0) in vec3 vertexPos;
layout (location = 1) in vec2 textureCoordinate;
layout (location = 2) in float face;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 fragPosWorld;
out vec3 fragNormal;
out vec2 textureCoord;
out flat uint faceIndex;

void main()
{
	textureCoord = textureCoordinate;
	faceIndex = uint(face);
	vec4 worldPos = model * vec4(vertexPos, 1);;
	fragPosWorld = worldPos.xyz;
	gl_Position = projection * view * worldPos;
    fragNormal = (transpose(inverse(model)) * vec4(faceIndexToNormal[faceIndex], 0)).xyz;
}