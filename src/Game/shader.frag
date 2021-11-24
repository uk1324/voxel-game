#version 430 core

uniform sampler2D sprite;

in vec2 texCoord;

void main()
{
	gl_FragColor = texture(sprite, texCoord);
}