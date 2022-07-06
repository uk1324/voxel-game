#version 430 core

uniform sampler2DArray blockTextureArray;

in flat uint textureIndex;
in vec2 texturePos;
in vec3 fragNormal;
in vec4 fragPosWorld;

layout (location = 0) out vec4 outPos;
layout (location = 1) out vec4 outNormal;
layout (location = 2) out vec4 outAlbedo;

void main()
{
	vec4 tex = texture(blockTextureArray, vec3(texturePos, textureIndex));

	if (tex.a == 0)
		discard;

	outAlbedo = vec4(tex.rgb, 1);
	outPos = fragPosWorld;
	outNormal = vec4(fragNormal, 1);
}