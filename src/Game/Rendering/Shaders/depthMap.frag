#version 430 core

uniform sampler2DArray blockTextureArray;

in vec2 texCoord;
in flat uint texIndex;

void main()
{
	vec4 tex = texture(blockTextureArray, vec3(texCoord, texIndex));
	if (tex.a == 0)
		discard;
}