#version 430 core

in vec3 fragPosWorld;
in vec3 fragColor;
in vec3 fragNormal;

layout (location = 0) out vec4 outPos;
layout (location = 1) out vec4 outNormal;
layout (location = 2) out vec4 outAlbedo;

void main()
{
	outAlbedo = vec4(fragColor, 1);
	outPos = vec4(fragPosWorld, 1);
	outNormal = vec4(fragNormal, 1);
}