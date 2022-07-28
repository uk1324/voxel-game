#version 430 core

uniform sampler2D inputColorTexture;
uniform sampler2D inputDepthTexture;

in vec2 texturePos;

out vec4 outputColor;

void main()
{
	vec3 color = texture(inputColorTexture, texturePos).xyz;
	outputColor = vec4(color, 1);	
	gl_FragDepth = texture(inputDepthTexture, texturePos).r;
}