#version 430 core

uniform mat4 view;
uniform mat4 projection;

layout (location = 0) in vec2 vertexPos;
layout (location = 1) in vec2 vertexTexturePos;
layout (location = 2) in vec4 instanceModelBasisX;
layout (location = 3) in vec4 instanceModelBasisY;
layout (location = 4) in vec4 instanceModelBasisZ;
layout (location = 5) in vec4 instanceModelBasisW;
layout (location = 6) in uint instanceTextureIndex;

out vec2 texturePos;
out flat uint textureIndex;
out vec3 fragPosWorld;

void main()
{
	texturePos = vertexTexturePos;
	textureIndex = instanceTextureIndex;
	vec4 posWorld = mat4(instanceModelBasisX, instanceModelBasisY, instanceModelBasisZ, instanceModelBasisW) * vec4(vertexPos, 0.0, 1.0);
	fragPosWorld = posWorld.xyz;
	gl_Position = projection * view * posWorld;
}