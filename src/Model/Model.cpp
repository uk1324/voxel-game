#include <Model/Model.hpp>
#include <Log/Log.hpp>
#include <Utils/Assertions.hpp>
#include <stb_image.h>

#include <iostream>
#include <filesystem>

Model::Model(std::string_view path)
{
	const Json::Value model = jsonFromFile(path);

	std::filesystem::path folder = std::filesystem::path(path).parent_path();

	try
	{
		int defaultScene = model.at("scene").getIntNumber();
		const Json::Value::ArrayType& rootNodes = model.at("scenes").getArray().at(defaultScene).at("nodes").getArray();
		const Json::Value::ArrayType& nodes = model.at("nodes").getArray();
		const Json::Value::ArrayType& meshes = model.at("meshes").getArray();

		for (auto node : rootNodes)
		{
			Json::Value::IntType nodeIndex = node.getIntNumber();
			root.children.push_back(parseNode(nodes, meshes, nodeIndex));
		}

		const Json::Value::ArrayType& buffers = model.at("buffers").getArray();

		for (const Json::Value& buffer : buffers)
		{
			const Json::Value::MapType& bufferInfo = buffer.getObject();
			std::string bufferData = stringFromFile((folder / bufferInfo.at("uri").getString()).string());
			size_t bufferDataLength = static_cast<size_t>(bufferInfo.at("byteLength").getIntNumber());
			ASSERT(bufferDataLength == bufferData.size());
			m_buffers.push_back(Gfx::VertexBuffer(bufferData.data(), bufferData.size()));
			m_buffers[0].bind();
			//glBufferSubData(GL_ARRAY_BUFFER, 0, bufferData.size(), bufferData.data());
		}

		const Json::Value::ArrayType& textures = model.at("images").getArray();

		for (const Json::Value& buffer : textures)
		{
			const Json::Value::MapType& bufferInfo = buffer.getObject();
			std::string bufferPath = (folder / bufferInfo.at("uri").getString()).string();
			stbi_set_flip_vertically_on_load(false);
			m_textures.push_back(Gfx::Texture(bufferPath));
		}

		this->meshes.push_back(Mesh(this->m_buffers, model, 0));
	}
	// Doesn't handle STL errors
	catch (const Json::Value::InvalidTypeAccess&)
	{
		LOG_FATAL("Failed to read glTF file");
	}
	catch (const Json::Value::OutOfRangeAccess&)
	{
		LOG_FATAL("Failed to read glTF file");
	}
}

Model::Node Model::parseNode(const Json::Value::ArrayType& nodes, const Json::Value::ArrayType& meshes, int nodeIndex)
{
	Json::Value::MapType node = nodes.at(nodeIndex).getObject();
	Node nodeData;

	auto children = node.find("children");
	if (children != node.end())
	{
		Json::Value::ArrayType childrenNodes = children->second.getArray();
		for (Json::Value node : childrenNodes)
		{
			Json::Value::IntType nodeIndex = node.getIntNumber();
			nodeData.children.push_back(parseNode(nodes, meshes, nodeIndex));
		}
	}

	auto mesh = node.find("mesh");
	if (mesh != node.end())
	{

	}

	return nodeData;
}

Model::Mesh::Mesh(std::vector<Gfx::VertexBuffer>& buffers, const Json::Value& model, int index)
{
	const Json::Value::MapType& mesh = model.at("meshes").getArray()[index].getObject();
	const Json::Value::ArrayType& accessors = model.at("accessors").getArray();
	const Json::Value::ArrayType& views = model.at("bufferViews").getArray();

	auto primitives = mesh.at("primitives").getArray()[0].at("attributes").getObject();
	std::cout << mesh.at("primitives").getArray()[0].at("attributes") << '\n';
	std::cout << mesh.at("primitives").getArray()[0].at("attributes").at("POSITION") << '\n';
	//auto position = primitives.find("POSITION");
	//if (position == primitives.end())
	{
		//int pos = position->second.getIntNumber();
		int pos = mesh.at("primitives").getArray()[0].at("attributes").at("POSITION").getIntNumber();
		auto accessor = accessors.at(pos);
		auto view = views.at(accessor.at("bufferView").getIntNumber());
		
		Gfx::BufferLayout layout;
		layout.dataType = static_cast<Gfx::ShaderDataType>(accessor.at("componentType").getIntNumber());
		const std::string& type = accessor.at("type").getString();

		if (type == "VEC3")
		{
			layout.dataTypeCountPerVertex = 3;
		}
		else
		{
			ASSERT_NOT_REACHED();
		}

		layout.isNormalized = false;
		layout.offset = view.at("byteOffset").getIntNumber() + accessor.at("byteOffset").getIntNumber();
		layout.stride = view.at("byteStride").getIntNumber();
		vao.bind();
		buffers.at(view.at("buffer").getIntNumber()).bind();
		vao.setAttribute(0, layout);
		std::cout << layout.offset << ' ' << layout.stride << '\n';
		std::cout << view.at("byteOffset").getIntNumber() << ' ' << accessor.at("byteOffset").getIntNumber() << '\n';
	}

	{
		//int pos = position->second.getIntNumber();
		int pos = mesh.at("primitives").getArray()[0].at("attributes").at("NORMAL").getIntNumber();
		auto accessor = accessors.at(pos);
		auto view = views.at(accessor.at("bufferView").getIntNumber());

		Gfx::BufferLayout layout;
		layout.dataType = static_cast<Gfx::ShaderDataType>(accessor.at("componentType").getIntNumber());
		const std::string& type = accessor.at("type").getString();

		if (type == "VEC3")
		{
			layout.dataTypeCountPerVertex = 3;
		}
		else
		{
			ASSERT_NOT_REACHED();
		}

		layout.isNormalized = false;
		layout.offset = view.at("byteOffset").getIntNumber() + accessor.at("byteOffset").getIntNumber();
		layout.stride = view.at("byteStride").getIntNumber();
		vao.bind();
		buffers.at(view.at("buffer").getIntNumber()).bind();
		vao.setAttribute(1, layout);
		std::cout << layout.offset << ' ' << layout.stride << '\n';
		std::cout << view.at("byteOffset").getIntNumber() << ' ' << accessor.at("byteOffset").getIntNumber() << '\n';
	}

	{
		//int pos = position->second.getIntNumber();
		int pos = mesh.at("primitives").getArray()[0].at("attributes").at("TEXCOORD_0").getIntNumber();
		auto accessor = accessors.at(pos);
		auto view = views.at(accessor.at("bufferView").getIntNumber());

		Gfx::BufferLayout layout;
		layout.dataType = static_cast<Gfx::ShaderDataType>(accessor.at("componentType").getIntNumber());
		const std::string& type = accessor.at("type").getString();

		if (type == "VEC2")
		{
			layout.dataTypeCountPerVertex = 2;
		}
		else
		{
			ASSERT_NOT_REACHED();
		}

		layout.isNormalized = false;
		layout.offset = view.at("byteOffset").getIntNumber() + accessor.at("byteOffset").getIntNumber();
		layout.stride = view.at("byteStride").getIntNumber();
		vao.bind();
		buffers.at(view.at("buffer").getIntNumber()).bind();
		vao.setAttribute(2, layout);
		std::cout << layout.offset << ' ' << layout.stride << '\n';
		std::cout << view.at("byteOffset").getIntNumber() << ' ' << accessor.at("byteOffset").getIntNumber() << '\n';
	}

	{
		int indices = mesh.at("primitives").getArray()[0].at("indices").getIntNumber();

		auto accessor = accessors.at(indices);
		auto view = views[accessor.at("bufferView").intNumber()];
		count = accessor.at("count").intNumber();
		offset = view.at("byteOffset").getIntNumber() + accessor.at("byteOffset").getIntNumber();
	}
}
