#pragma once

#include <Engine/Graphics/Vao.hpp>
#include <Engine/Graphics/Texture.hpp>
#include <Utils/FileIo.hpp>
#include <Utils/SmartPointers.hpp>
#include <Math/Mat4.hpp>

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

	struct Node
	{
		std::vector<Node*> children;
		Node* parent;
		Mat4 transform;
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
	
	std::vector<Node*> children;

	std::vector<Node*> joints;

	std::vector<Mat4> inverseBindMatrices;
};