#version 430 core

layout (location = 0) in uint vert;

uniform mat4 model;

out flat uint texIndex;
out vec2 texCoord;

vec2 texCoords[4] = {
	vec2(0, 0),
	vec2(1, 0),
	vec2(0, 1),
	vec2(1, 1)
};

#define X_OFFSET 0
#define Y_OFFSET 5
#define Z_OFFSET Y_OFFSET + 5
#define TEXTURE_INDEX_OFFSET Z_OFFSET + 5
#define TEXTURE_POS_INDEX_OFFSET TEXTURE_INDEX_OFFSET + 10

#define X_MASK ~((~0) << (X_OFFSET + 5))
#define Y_MASK ~((~0) << (Y_OFFSET + 5))
#define Z_MASK ~((~0) << (Z_OFFSET + 5))
#define TEXTURE_INDEX_MASK ~((~0) << (TEXTURE_INDEX_OFFSET + 5))
#define TEXTURE_POS_INDEX_MASK ~((~0) << (TEXTURE_POS_INDEX_OFFSET + 5))

#define CHUNK_SIZE 16

void main()
{
	vec3 vertexPosition = vec3(
		(vert & X_MASK) >> X_OFFSET,
		(vert & Y_MASK) >> Y_OFFSET,
		(vert & Z_MASK) >> Z_OFFSET
	);

	texIndex = (vert & TEXTURE_INDEX_MASK) >> TEXTURE_INDEX_OFFSET;

	gl_Position = (model * vec4(vertexPosition, 1.0));
	texCoord = texCoords[(vert & TEXTURE_POS_INDEX_MASK) >> TEXTURE_POS_INDEX_OFFSET];
}