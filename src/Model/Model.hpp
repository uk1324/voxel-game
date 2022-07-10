#pragma once

#include <Engine/Graphics/Vao.hpp>
#include <Engine/Graphics/Texture.hpp>
#include <Utils/FileIo.hpp>
#include <Utils/SmartPointers.hpp>
#include <Math/Mat4.hpp>
#include <Math/Quat.hpp>

#include <string_view>
#include <vector>

class Model
{
public:
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
		std::vector<Quat> rotation;
		std::vector<Vec3> translation;
		std::vector<Vec3> scale;
	};

	struct Node
	{
		std::vector<Node*> children;
		Node* parent;
		Mat4 transform;
		Mat4 output = Mat4::identity;
	};

public:
	void draw();

private:
	static size_t parseTypeCount(const Json::Value::StringType& type);

	size_t sizeOfShaderDataType(ShaderDataType type);

public:
	std::vector<Vbo> buffers;
	std::vector<Mesh> meshes;
	std::vector<Texture> textures;
	std::vector<Node> nodes;
	
	// One advantage of using indices instead of pointers is that smaller integer times like uint16_t or uint32_t can be used.
	std::vector<Node*> children;

	std::vector<Node*> joints;

	std::vector<Mat4> inverseBindMatrices;

	std::vector<KeyFrame> keyframes;
};