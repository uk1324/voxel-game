#version 430 core

#include "../chunkCommon.glsl"
#include "../Util/noise.glsl"

layout (location = 0) in uint vert;

// Rename to world to model
// world to clip
// world to view.
uniform mat4 model;
uniform mat4 projection;
uniform mat4 camera;

uniform float time;

out flat uint textureIndex;
out vec2 texturePos;
out vec3 fragNormal;
out vec3 fragPosWorld;

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
	// This doesn't change the normal's rotation based on the model matrix.
	// It might be better to just pass the chunks position then it won't be necessary.
	fragNormal = normals[normalIndex];
	fragPosWorld = (model * vec4(vertPos, 1)).xyz;

	float waveScale = 0.2;
	fragPosWorld.y -= waveScale;
    fragPosWorld.y += perlin01((vec2(fragPosWorld.xz) + vec2(time)) * 0.5) * waveScale;

	gl_Position = projection * camera * vec4(fragPosWorld, 1);
}