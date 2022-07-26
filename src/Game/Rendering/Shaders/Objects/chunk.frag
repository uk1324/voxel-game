#version 430 core

uniform sampler2DArray blockTextureArray;

in flat uint textureIndex;
in vec2 texturePos;
in vec3 fragNormal;
in vec3 fragPosWorld;

layout (location = 0) out vec4 outPos;
layout (location = 1) out vec4 outNormal;
layout (location = 2) out vec4 outAlbedo;

void main()
{
	vec4 color = texture(blockTextureArray, vec3(texturePos, textureIndex));

	if (color.a == 0)
		discard;

	outAlbedo = vec4(color.rgb, 1);
	outPos = vec4(fragPosWorld, 1);
	outNormal = vec4(fragNormal, 1);
}