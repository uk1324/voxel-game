#version 430 core

uniform sampler2D textureSampler;

in vec2 textureCoord;

out vec4 fragColor;

void main()
{
	fragColor = texture(textureSampler, textureCoord);
}