#pragma once

#include <Engine/Graphics/Vao.hpp>
#include <Engine/Graphics/Texture.hpp>
#include <Utils/FileIo.hpp>
#include <Utils/SmartPointers.hpp>
#include <Math/Mat4.hpp>
#include <Math/Quat.hpp>

#include <string_view>
#include <vector>

/*
Don't know if it is better to use pointers or indices. I won't be modyfing the vectors but if I do the pointers might get invalidated.
When using indices the compiler has to do one
lea indexRegister, [index+index*2]
sal indexRegister, 5 and then lea;
lea returnRegister, [arrayPtrRegister+indexRegister]
It basically does (index * 3) << 5 Which is the same as index * 96 because 96 is the size of the struct.
I don't know how much faster that is. The compiler probably can't optimize the array accesses away because
in skeletal animation I won't just be using a loop to iterate over the elemnts I will also need things like the parent
transform which would probably require more jumping around in memory.
The advantage of using indices instead of pointers is that smaller integer sizes like uint16_t or uint32_t can be used
so more nodes can fit on a cache line, which is probably more important for performance.
It is also more comfortable when creating new arrays which map to the indices of the nodes.
*/
struct Model
{
	struct Mesh
	{
		Vao vao;
		Texture& texture;
		Vbo& indicesVbo;
		size_t indicesByteOffset;
		size_t indicesCount;
		ShaderDataType indexType;
	};

	struct KeyFrame
	{
		float time;
		// Index into these vector represents the node index.
		std::vector<Quat> rotation;
		std::vector<Vec3> translation;
		std::vector<Vec3> scale;
	};

	struct Node
	{
		std::vector<Node*> children;
		// Then transform isn't very useful when importing animations.
		// It represen't the default pose without an animation playing.
		// Only the transform for the root node is useful when playing an animation.
		Mat4 defaultPosTransform; // Not bind pose
	};

	size_t nodeIndex(const Node* node) const;

	std::vector<Vbo> buffers;
	std::vector<Mesh> meshes;
	std::vector<Texture> textures;
	std::vector<Node> nodes;
	std::vector<std::vector<KeyFrame>> animations;
	
	std::vector<Node*> children;

	std::vector<Node*> joints;

	std::vector<Mat4> inverseBindMatrices;

	Node* rootNode;
};