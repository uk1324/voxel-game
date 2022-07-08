#version 430 core

layout (location = 0) in vec3 vertexPos;
layout (location = 1) in vec3 vertexNormal;
layout (location = 2) in vec2 vertexUv;
layout (location = 3) in vec4 vertexJoints;
layout (location = 4) in vec4 vertexWeights;

uniform mat4 jointMatrices[16];
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out VsOut
{
	vec3 normal;
	vec2 uv;
	vec3 worldPos;
} o;

out vec3 wp;

void main()
{
	mat4 skinMat =
		vertexWeights.x * jointMatrices[int(vertexJoints.x)] +
		vertexWeights.y * jointMatrices[int(vertexJoints.y)] +
		vertexWeights.z * jointMatrices[int(vertexJoints.z)] +
		vertexWeights.w * jointMatrices[int(vertexJoints.w)];

	vec4 worldPos = model * skinMat * vec4(vertexPos, 1.0);
	o.normal = vertexNormal;
	o.uv = vertexUv;
	o.worldPos = worldPos.xyz;
	gl_Position = projection * view * worldPos;
}