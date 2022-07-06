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
#define Y_OFFSET 5
#define Z_OFFSET Y_OFFSET + 5
#define TEXTURE_INDEX_OFFSET Z_OFFSET + 5
#define TEXTURE_POS_INDEX_OFFSET TEXTURE_INDEX_OFFSET + 10
#define NORMAL_INDEX_OFFSET TEXTURE_POS_INDEX_OFFSET + 2

#define X_MASK ~((~0) << (X_OFFSET + 5))
#define Y_MASK ~((~0) << (Y_OFFSET + 5))
#define Z_MASK ~((~0) << (Z_OFFSET + 5))
#define TEXTURE_INDEX_MASK ~((~0) << (TEXTURE_INDEX_OFFSET + 10))
#define TEXTURE_POS_INDEX_MASK ~((~0) << (TEXTURE_POS_INDEX_OFFSET + 2))
#define NORMAL_INDEX_MASK ~((~0) << (NORMAL_INDEX_OFFSET + 3))

#define CHUNK_SIZE 16