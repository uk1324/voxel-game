#version 430 core

uniform sampler2D textureSampler;

in VsOut
{
	vec3 normal;
	vec2 uv;
	vec3 worldPos;
} i;

layout (location = 0) out vec4 outPos;
layout (location = 1) out vec4 outNormal;
layout (location = 2) out vec4 outAlbedo;

void main()
{
	outPos = vec4(i.worldPos, 1);
	outAlbedo = vec4(texture(textureSampler, i.uv).rgb, 1);	
	outNormal = vec4(i.normal, 1);
}