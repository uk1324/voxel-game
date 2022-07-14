#version 430 core

layout (location = 0) in vec3 vertexPos;
layout (location = 1) in vec3 vertexNormal;
layout (location = 2) in vec2 vertexUv;
layout (location = 3) in vec4 vertexJoints;
layout (location = 4) in vec4 vertexWeights;
layout (location = 5) in vec4 vertexTangent;

uniform mat4 jointMatrices[64];
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out VsOut
{
	vec2 uv;
	vec3 worldPos;
	mat3 normalMat;
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
	mat4 contravariantModel = transpose(inverse(model));
	vec3 normal = (contravariantModel * vec4(vertexNormal, 0)).xyz;
	vec3 tangent = (contravariantModel * vec4(vertexTangent.xyz, 0)).xyz;
	// Reorthogonalize the tangent vector using the Gram–Schmidt process.
	tangent = normalize(tangent - dot(tangent, normal) * normal);
	// In glTF the w component of the tagnent specifies the handedness of the coordinate space.
	vec3 bitangent = vertexTangent.w < 0
		? cross(normal, tangent)
		: cross(tangent, normal);
	o.normalMat = mat3(tangent, bitangent, normal);

	o.uv = vertexUv;
	o.worldPos = worldPos.xyz;
	gl_Position = projection * view * worldPos;
}