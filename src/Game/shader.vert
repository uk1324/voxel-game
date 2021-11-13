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

uniform mat4 model;
uniform mat4 projection;
uniform mat4 rotation;

out vec2 textureCoord;

void main()
{
	gl_Position = projection * rotation * model * vec4(vertex, 1.0);
}