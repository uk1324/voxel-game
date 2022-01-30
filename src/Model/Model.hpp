#pragma once

#include <Engine/Graphics/Vao.hpp>
#include <Engine/Graphics/Texture.hpp>
#include <Utils/FileIo.hpp>
#include <Utils/SmartPointers.hpp>

#include <string_view>
#include <vector>

class Model
{
public:
	class SceneData
	{

	};

	class Node
	{
	public:
		std::vector<Node> children;
	};

	class Mesh
	{
	public:
		Mesh(std::vector<Vbo>& buffers, const Json::Value& model, int index);
		
	public:
		Vao vao;
		size_t offset;
		size_t count;
	};

public:
	Model(std::string_view path);

private:
	// This could be static
	Node parseNode(const Json::Value::ArrayType& nodes, const Json::Value::ArrayType& meshes, int nodeIndex);

public:
	// Make this public
	Node root;

	std::vector<Mesh> meshes;

	const Json::Value::ArrayType* accessors;
	const Json::Value::ArrayType* bufferViews;

	Vao vao;
	std::vector<Vbo> m_buffers;
	std::vector<Texture> m_textures;
};