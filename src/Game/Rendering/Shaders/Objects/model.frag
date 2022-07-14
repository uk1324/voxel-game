#version 430 core

uniform sampler2D textureSampler;
uniform sampler2D normalMap;

in VsOut
{
	vec2 uv;
	vec3 worldPos;
	mat3 normalMat;
} i;
 
layout (location = 0) out vec4 outPos;
layout (location = 1) out vec4 outNormal;
layout (location = 2) out vec4 outAlbedo;

void main()
{
	outPos = vec4(i.worldPos, 1);
	outAlbedo = vec4(texture(textureSampler, i.uv).rgb, 1);	
	// Rgb values are in range [0, 1] so it has to be converted to [-1, 1].
	vec3 normal = normalize(texture(normalMap, i.uv).xyz * 2 - 1);
	// Normals in normal maps are in a space where the normal of the surface is the forward vector, the other basis
	// point in the direction of the UV up and right vectors.
	outNormal = vec4(i.normalMat * normal, 1);
}