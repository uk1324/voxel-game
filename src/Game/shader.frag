#version 430 core

in vec2 textureCoord;

uniform sampler2D sprite;

void main()
{
	gl_FragColor = texture(sprite, textureCoord);
}