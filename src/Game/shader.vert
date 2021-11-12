#version 430 core

layout (location = 0) in vec2 vertex;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 rotation;

void main()
{
	gl_Position = projection * rotation * model * vec4(vertex, 0.0, 1.0);
}