#version 430 core

uniform sampler2DArray blockTextureArray;

in vec2 texCoordG;
in flat uint texIndexG;

void main()
{
	vec4 tex = texture(blockTextureArray, vec3(texCoordG, texIndexG));
	if (tex.a == 0)
		discard;
}