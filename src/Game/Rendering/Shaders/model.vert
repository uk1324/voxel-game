#version 430 core

layout (location = 0) in vec3 vertexPos;
layout (location = 1) in vec3 vertexNormal;
layout (location = 2) in vec2 vertexUv;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out VsOut
{
	vec3 normal;
	vec2 uv;
} o;

void main()
{
	gl_Position = projection * view * model * vec4(vertexPos, 1.0);
	o.normal = vertexNormal;
	o.uv = vertexUv;
}