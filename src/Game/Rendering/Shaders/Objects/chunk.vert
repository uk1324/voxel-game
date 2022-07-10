#version 430 core

layout (location = 0) in uint vert;

vec3 normals[] = {
 	vec3(1, 0, 0), // Left
 	vec3(-1, 0, 0), // Right
 	vec3(0, 1, 0), // Top
 	vec3(0, -1, 0), // Bottom
 	vec3(0, 0, 1), // Back
 	vec3(0, 0, -1) // Front
 };

vec2 texCoords[] = {
	vec2(0, 0),
	vec2(1, 0),
	vec2(0, 1),
	vec2(1, 1)
};

#define X_OFFSET 0
#define Y_OFFSET 4
#define Z_OFFSET Y_OFFSET + 7
#define TEXTURE_INDEX_OFFSET Z_OFFSET + 4
#define TEXTURE_POS_INDEX_OFFSET TEXTURE_INDEX_OFFSET + 10
#define NORMAL_INDEX_OFFSET TEXTURE_POS_INDEX_OFFSET + 2

#define X_MASK ~((~0) << (X_OFFSET + 4))
#define Y_MASK ~((~0) << (Y_OFFSET + 7))
#define Z_MASK ~((~0) << (Z_OFFSET + 4))
#define TEXTURE_INDEX_MASK ~((~0) << (TEXTURE_INDEX_OFFSET + 10))
#define TEXTURE_POS_INDEX_MASK ~((~0) << (TEXTURE_POS_INDEX_OFFSET + 2))
#define NORMAL_INDEX_MASK ~((~0) << (NORMAL_INDEX_OFFSET + 3))

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
	// This doesn't change the normal's rotation based on the model matrix.
	// It might be better to just pass the chunks position then it won't be necessary.
	fragNormal = normals[normalIndex];
	fragPosWorld = (model * vec4(vertPos, 1));
	gl_Position = projection * camera * model * vec4(vertPos, 1.0);
}