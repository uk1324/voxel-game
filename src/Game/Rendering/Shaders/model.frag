#version 430 core

in VsOut
{
	vec3 normal;
	vec2 uv;
} i;

out vec4 outColor;

void main()
{
	outColor = vec4(1, 0, 0, 1);
}