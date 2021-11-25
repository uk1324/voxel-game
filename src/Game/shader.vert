#version 430 core

//layout (location = 0) in vec3 vertex;
//layout (location = 1) in vec2 spriteCoord;
//
//uniform mat4 model;
//uniform mat4 projection;
//uniform mat4 rotation;
//
//out vec2 textureCoord;
//
//void main()
//{
//	textureCoord = spriteCoord;
//	gl_Position = projection * rotation * model * vec4(vertex, 1.0);
//}

layout (location = 0) in vec3 vertex;
layout (location = 1) in vec2 textureCoord;
layout (location = 2) in uint textureIndex;
layout (location = 3) in uint normalIndex;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 camera;

out flat uint texIndex;
out vec2 texCoord;
out vec3 normal;

vec3 normals[6] = {
	vec3(1, 0, 0), // Left
	vec3(-1, 0, 0), // Right
	vec3(0, 1, 0), // Top
	vec3(0, -1, 0), // Bottom
	vec3(0, 0, 1), // Back
	vec3(0, 0, -1) // Front
};

void main()
{
	gl_Position = projection * camera * model * vec4(vertex, 1.0);
	texCoord = textureCoord;
	texIndex = textureIndex;
	normal = normals[normalIndex];
}