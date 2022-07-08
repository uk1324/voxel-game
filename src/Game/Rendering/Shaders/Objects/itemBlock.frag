#version 430 core

vec3 faceIndexToNormal[] = {
 	vec3(0, 0, -1), // Front
 	vec3(0, 0, 1), // Back
 	vec3(-1, 0, 0), // Right
	vec3(1, 0, 0), // Left
 	vec3(0, -1, 0), // Bottom
 	vec3(0, 1, 0) // Top
};

uniform sampler2DArray textureArraySampler;
uniform uint faceTextureIndex[6];

in vec3 fragPosWorld;
in vec2 textureCoord;
in flat uint faceIndex;

layout (location = 0) out vec4 outPos;
layout (location = 1) out vec4 outNormal;
layout (location = 2) out vec4 outAlbedo;

void main()
{
	outAlbedo = texture(textureArraySampler, vec3(textureCoord, faceTextureIndex[faceIndex]));
    outPos = vec4(fragPosWorld, 1);
    outNormal = vec4(faceIndexToNormal[faceIndex], 1);
}