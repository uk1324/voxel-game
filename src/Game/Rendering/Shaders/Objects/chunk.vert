#version 430 core
#include "chunksCommon.glsl"

layout (location = 0) in uint vert;

// Rename to world to model
// world to clip
// world to view.
uniform mat4 model;
uniform mat4 projection;
uniform mat4 camera;

out flat uint textureIndex;
out vec2 texturePos;
out vec3 fragNormal;
out vec4 fragPosWorld;

void main()
{
	vec3 vertPos = vec3(
		(vert & X_MASK) >> X_OFFSET,
		(vert & Y_MASK) >> Y_OFFSET,
		(vert & Z_MASK) >> Z_OFFSET
	);
	uint normalIndex = (vert & NORMAL_INDEX_MASK) >> NORMAL_INDEX_OFFSET;

	textureIndex = (vert & TEXTURE_INDEX_MASK) >> TEXTURE_INDEX_OFFSET;
	texturePos = texCoords[(vert & TEXTURE_POS_INDEX_MASK) >> TEXTURE_POS_INDEX_OFFSET];	
	fragNormal = normals[normalIndex];
	fragPosWorld = (model * vec4(vertPos, 1));
	gl_Position = projection * camera * model * vec4(vertPos, 1.0);
}