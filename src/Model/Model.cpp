#include <Model/Model.hpp>
#include <Log/Log.hpp>
#include <Utils/Assertions.hpp>

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
			float* data = reinterpret_cast<float*>(bufferData.data());
			for (int i = 0; i < 576 / 4; i++)
			{
				if (i % 3 == 0) std::cout << '|';
				if (i * 4 == 288) std::cout << '\n' << '\n';
				std::cout << data[i] << ' ';
			}
			data += 576 / 4;
			GL_UNSIGNED_SHORT;
			for (int i = 0; i < bufferData.size() / 2 - 576 / 4; i++)
			{
				std::cout << reinterpret_cast<unsigned short*>(data)[i] << ' ';
				//if (i % 3 == 0) std::cout << '|';
			}
			size_t bufferDataLength = static_cast<size_t>(bufferInfo.at("byteLength").getIntNumber());
			ASSERT(bufferDataLength == bufferData.size());
			m_buffers.push_back(Gfx::VertexBuffer(bufferData.data(), bufferData.size()));
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
	
	vao.bind();

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
		buffers.at(view.at("buffer").getIntNumber()).bind();
		vao.setAttribute(0, layout);
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
