#version 430 core

uniform sampler2DArray textureArraySampler;
uniform uint faceTextureIndex[6];

in vec2 textureCoord;

out vec4 fragColor;
in flat uint faceIndex;

#define DARKEN_MULTIPLIER 0.5

void main()
{
	vec4 color = texture(textureArraySampler, vec3(textureCoord, faceTextureIndex[faceIndex]));

	// Left side
	if (faceIndex == 3)
	{
		color *= vec4(vec3(DARKEN_MULTIPLIER), 1.0);
	}

	fragColor = color;
}