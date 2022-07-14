#version 430 core

layout (location = 0) in vec3 vertexPos;
layout (location = 1) in vec3 vertexColor;
layout (location = 2) in vec3 vertexNormal;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

out vec3 fragColor;
out vec3 fragPosWorld;
out vec3 fragNormal;

void main()
{
	vec4 posWorld = model * vec4(vertexPos, 1.0);
	fragPosWorld = posWorld.xyz;
	fragColor = vertexColor;
	fragNormal = (transpose(inverse(model)) * vec4(vertexNormal, 0)).xyz;
	gl_Position = projection * view * posWorld;
}