#version 430 core

uniform sampler2D texture;

in vec2 texturePos;

out vec4 outColor;

void main()
{
	outColor = vec4(vec3(texture2D(texture, texturePos).r), 1);
}