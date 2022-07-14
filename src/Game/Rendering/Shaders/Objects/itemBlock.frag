#version 430 core

uniform sampler2DArray textureArraySampler;
uniform uint faceTextureIndex[6];

in vec3 fragPosWorld;
in vec3 fragNormal;
in vec2 textureCoord;
in flat uint faceIndex;

layout (location = 0) out vec4 outPos;
layout (location = 1) out vec4 outNormal;
layout (location = 2) out vec4 outAlbedo;

void main()
{
	outAlbedo = texture(textureArraySampler, vec3(textureCoord, faceTextureIndex[faceIndex]));
    outPos = vec4(fragPosWorld, 1);
	outNormal = vec4(fragNormal, 1);
}