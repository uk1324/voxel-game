#version 430 core

out vec4 fragColor;

uniform sampler2D txt;

in vec2 texCoord;

void main()
{
	// fragColor = vec4(0, 1, 0, 1);
	fragColor = texture(txt, (texCoord + vec2(1, 1)) / 2);
}