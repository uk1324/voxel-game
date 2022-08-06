#version 430 core

uniform sampler2DArray blockTextureArray;
uniform vec3 directionalLightDir;

in vec3 fragPosWorld;
in vec2 texturePos;
in flat uint textureIndex;

layout (location = 0) out vec4 outPos;
layout (location = 1) out vec4 outNormal;
layout (location = 2) out vec4 outAlbedo;

void main()
{
	vec4 color = texture(blockTextureArray, vec3(texturePos / 8 + 0.5, textureIndex));
	if (color.a == 0)
		discard;
	outAlbedo = color;
	outPos = vec4(fragPosWorld, 1);
	outNormal = vec4(-directionalLightDir, 1);
}