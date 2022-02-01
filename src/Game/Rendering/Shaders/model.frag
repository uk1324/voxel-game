#version 430 core

uniform sampler2D textureSampler;

in VsOut
{
	vec3 normal;
	vec2 uv;
} i;

out vec4 outColor;

void main()
{
	outColor = texture(textureSampler, i.uv);
}