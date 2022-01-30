#version 430 core

layout (location = 0) in vec3 i_vertex;
layout (location = 1) in vec3 i_normal;
layout (location = 2) in vec2 i_uv;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out VS_OUT
{
	vec3 normal;
	vec2 uv;
} o;

void main()
{
	gl_Position = projection * view * model * vec4(i_vertex, 1.0);
	o.normal = i_normal;
	o.uv = i_uv;
}