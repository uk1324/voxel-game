#version 430 core

uniform sampler2DArray textureArraySampler;
uniform uint faceTextureIndex[6];

in vec2 textureCoord;

out vec4 fragColor;
in flat uint faceIndex;

void main()
{
	fragColor = texture(textureArraySampler, vec3(textureCoord, faceTextureIndex[faceIndex]));;
}