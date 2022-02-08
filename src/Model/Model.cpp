#include <Model/Model.hpp>
#include <Log/Log.hpp>
#include <Utils/Assertions.hpp>
#include <stb_image.h>

#include <iostream>
#include <filesystem>

#if 0

Model::Model(std::string_view path)
{
	const Json::Value file = jsonFromFile(path);

	// Make more things const and remove the pointless copies.
	std::filesystem::path folder = std::filesystem::path(path).parent_path();
	try
	{
		for (Json::Value buffer : file.at("buffers").array())
		{
			// Data might not be aligned properly.
 			std::string data = stringFromFile((folder / buffer.at("uri").string()).string());
			Json::Value::IntType size = buffer.at("byteLength").intNumber();
			if (data.size() != size)
				LOG_FATAL("buffer data size doesn't match the specified size");
			buffers.push_back(Vbo(data.data(), data.size()));
		}

		Json::Value images = file.at("images");
		for (Json::Value texture : file.at("textures").array())
		{
			Json::Value image = images.at(texture.at("source").intNumber());
			textures.push_back(Texture((folder / image.at("uri").string()).string()));
		}

		Json::Value bufferViews = file.at("bufferViews");
		Json::Value accessors = file.at("accessors");
		auto loadVboAttribute = [&bufferViews, &accessors, this](size_t vboIndex, Json::Value::IntType accessorIndex) {
			Json::Value accessor = accessors.at(accessorIndex);
			Json::Value bufferView = bufferViews.at(accessor.at("bufferView").intNumber());

			Vbo* vbo = &buffers.at(bufferView.at("buffer").intNumber());
			Json::Value::IntType bufferViewByteOffset = bufferView.at("byteOffset").intNumber();
			Json::Value::IntType accessorByteOffset = accessor.contains("byteOffset") ? accessor.at("byteOffset").intNumber() : 0;

			Json::Value::IntType byteOffset = bufferViewByteOffset + accessorByteOffset;

			auto componentType = static_cast<ShaderDataType>(accessor.at("componentType").intNumber());
			auto componentTypePerVertexCount = parseTypeCount(accessor.at("type").string());

			Json::Value::IntType stride = bufferView.contains("stride")
				? bufferView.at("byteStride").intNumber()
				: componentTypePerVertexCount * sizeOfShaderDataType(componentType);

			vbo->bind();
			Vao::setAttribute(vboIndex, componentType, componentTypePerVertexCount, false, stride, byteOffset);
		};
		for (Json::Value mesh : file.at("meshes").array())
		{
			// Allow only a single set of primitives per mesh for now.
			Json::Value primitives = mesh.at("primitives").at(0);
			Json::Value attributes = primitives.at("attributes");
			
			Vao vao;
			vao.bind();
			loadVboAttribute(0, attributes.at("POSITION").intNumber());
			loadVboAttribute(1, attributes.at("NORMAL").intNumber());
			loadVboAttribute(2, attributes.at("TEXCOORD_0").intNumber());

			size_t count;
			size_t byteOffset;
			ShaderDataType indexType;

			Json::Value::IntType indicesAccessorIndex = primitives.at("indices").intNumber();
			Json::Value indicesAccessor = accessors.at(indicesAccessorIndex);
			Json::Value indicesBufferView = bufferViews.at(indicesAccessor.at("bufferView").intNumber());

			Json::Value::IntType bufferViewByteOffset = indicesBufferView.at("byteOffset").intNumber();
			Json::Value::IntType accessorByteOffset = indicesAccessor.contains("byteOffset") ? indicesAccessor.at("byteOffset").intNumber() : 0;

			byteOffset = bufferViewByteOffset + accessorByteOffset;
			indexType = static_cast<ShaderDataType>(indicesAccessor.at("componentType").intNumber());
			count = indicesAccessor.at("count").intNumber();
			Vbo* vbo = &buffers.at(indicesBufferView.at("buffer").intNumber());

			meshes.push_back(Mesh{ std::move(vao), vbo, byteOffset, count, indexType });
		}
	}
	catch (const Json::Value::InvalidTypeAccess&)
	{
		LOG_FATAL("Failed to read glTF file");
	}
	catch (const Json::Value::OutOfRangeAccess&)
	{
		LOG_FATAL("Failed to read glTF file");
	}
	catch (const std::out_of_range&)
	{
		LOG_FATAL("Failed to read glTF file");
	}

	//try
	//{
	//	const auto defaultSceneIndex = model.at("scene").intNumber();
	//	const Json::Value::ArrayType& rootNodes = model.at("scenes").array().at(defaultSceneIndex).at("nodes").array();
	//	const Json::Value::ArrayType& nodes = model.at("nodes").array();
	//	const Json::Value::ArrayType& meshes = model.at("meshes").array();

	//	for (auto node : rootNodes)
	//	{
	//		Json::Value::IntType nodeIndex = node.intNumber();
	//		root.children.push_back(parseNode(nodes, meshes, nodeIndex));
	//	}

	//	const Json::Value::ArrayType& buffers = model.at("buffers").array();

	//	for (const Json::Value& buffer : buffers)
	//	{
	//		const Json::Value::MapType& bufferInfo = buffer.object();
	//		std::string bufferData = stringFromFile((folder / bufferInfo.at("uri").string()).string());
	//		size_t bufferDataLength = static_cast<size_t>(bufferInfo.at("byteLength").intNumber());
	//		ASSERT(bufferDataLength == bufferData.size());
	//		m_buffers.push_back(Vbo(bufferData.data(), bufferData.size()));
	//	}

	//	const Json::Value::ArrayType& textures = model.at("images").array();

	//	for (const Json::Value& buffer : textures)
	//	{
	//		const Json::Value::MapType& bufferInfo = buffer.object();
	//		std::string bufferPath = (folder / bufferInfo.at("uri").string()).string();
	//		stbi_set_flip_vertically_on_load(false);
	//		m_textures.push_back(Texture(bufferPath));
	//	}

	//	this->meshes.push_back(Mesh(this->m_buffers, model, 0));
	//}
	//// Doesn't handle STL errors
	//catch (const Json::Value::InvalidTypeAccess&)
	//{
	//	LOG_FATAL("Failed to read glTF file");
	//}
	//catch (const Json::Value::OutOfRangeAccess&)
	//{
	//	LOG_FATAL("Failed to read glTF file");
	//}
}

#endif // 0

void Model::draw()
{

}

size_t Model::parseTypeCount(const Json::Value::StringType& type)
{
	if (type == "SCALAR")
		return 1;
	if (type == "VEC2")
		return 2;
	if (type == "VEC3")
		return 3;

	ASSERT_NOT_REACHED();
	return 0;
}

size_t Model::sizeOfShaderDataType(ShaderDataType type)
{
	switch (type)
	{
		case ShaderDataType::Float:
			return 4;
	}

	ASSERT_NOT_REACHED();
	return 0;
}
//void Model::setAccessor(Vao& vao, size_t index, Json::Value accessor, Json::Value,)
//{
//	Json::Value::IntType positionAccessorIndex = attributes.at("POSITION").intNumber();
//	Json::Value positionAccessor = accessors.at(positionAccessorIndex);
//	Json::Value positionBufferView = bufferViews.at(positionAccessor.at("bufferView").intNumber());
//	Vbo* buffer = &buffers.at(positionBufferView.at("buffer").intNumber());
//	Json::Value::IntType bufferViewByteOffset = positionBufferView.at("byteOffset").intNumber();
//	Json::Value::IntType accessorByteOffset = positionAccessor.at("byteOffset").intNumber();
//	Json::Value::IntType stride = positionBufferView.at("byteStride").intNumber();
//}

//Model::Node Model::parseNode(const Json::Value::ArrayType& nodes, const Json::Value::ArrayType& meshes, int nodeIndex)
//{
//	Json::Value::MapType node = nodes.at(nodeIndex).object();
//	Node nodeData;
//
//	auto children = node.find("children");
//	if (children != node.end())
//	{
//		Json::Value::ArrayType childrenNodes = children->second.array();
//		for (Json::Value node : childrenNodes)
//		{
//			Json::Value::IntType nodeIndex = node.intNumber();
//			nodeData.children.push_back(parseNode(nodes, meshes, nodeIndex));
//		}
//	}
//
//	auto mesh = node.find("mesh");
//	if (mesh != node.end())
//	{
//
//	}
//
//	return nodeData;
//}

//Model::Mesh::Mesh(std::vector<Vbo>& buffers, const Json::Value& model, int index)
//{
//	const Json::Value::MapType& mesh = model.at("meshes").array()[index].object();
//	const Json::Value::ArrayType& accessors = model.at("accessors").array();
//	const Json::Value::ArrayType& views = model.at("bufferViews").array();
//
//	auto primitives = mesh.at("primitives").array()[0].at("attributes").object();
//	std::cout << mesh.at("primitives").array()[0].at("attributes") << '\n';
//	std::cout << mesh.at("primitives").array()[0].at("attributes").at("POSITION") << '\n';
//	//auto position = primitives.find("POSITION");
//	//if (position == primitives.end())
//	{
//		//int pos = position->second.getIntNumber();
//		int pos = mesh.at("primitives").array()[0].at("attributes").at("POSITION").intNumber();
//		auto accessor = accessors.at(pos);
//		auto view = views.at(accessor.at("bufferView").intNumber());
//		
//		BufferLayout layout;
//		layout.dataType = static_cast<ShaderDataType>(accessor.at("componentType").intNumber());
//		const std::string& type = accessor.at("type").string();
//
//		if (type == "VEC3")
//		{
//			layout.dataTypeCountPerVertex = 3;
//		}
//		else
//		{
//			ASSERT_NOT_REACHED();
//		}
//
//		layout.isNormalized = false;
//		layout.offset = view.at("byteOffset").intNumber() + accessor.at("byteOffset").intNumber();
//		layout.stride = view.at("byteStride").intNumber();
//		vao.bind();
//		buffers.at(view.at("buffer").intNumber()).bind();
//		vao.setAttribute(0, layout);
//		std::cout << layout.offset << ' ' << layout.stride << '\n';
//		std::cout << view.at("byteOffset").intNumber() << ' ' << accessor.at("byteOffset").intNumber() << '\n';
//	}
//
//	{
//		//int pos = position->second.getIntNumber();
//		int pos = mesh.at("primitives").array()[0].at("attributes").at("NORMAL").intNumber();
//		auto accessor = accessors.at(pos);
//		auto view = views.at(accessor.at("bufferView").intNumber());
//
//		BufferLayout layout;
//		layout.dataType = static_cast<ShaderDataType>(accessor.at("componentType").intNumber());
//		const std::string& type = accessor.at("type").string();
//
//		if (type == "VEC3")
//		{
//			layout.dataTypeCountPerVertex = 3;
//		}
//		else
//		{
//			ASSERT_NOT_REACHED();
//		}
//
//		layout.isNormalized = false;
//		layout.offset = view.at("byteOffset").intNumber() + accessor.at("byteOffset").intNumber();
//		layout.stride = view.at("byteStride").intNumber();
//		vao.bind();
//		buffers.at(view.at("buffer").intNumber()).bind();
//		vao.setAttribute(1, layout);
//		std::cout << layout.offset << ' ' << layout.stride << '\n';
//		std::cout << view.at("byteOffset").intNumber() << ' ' << accessor.at("byteOffset").intNumber() << '\n';
//	}
//
//	{
//		//int pos = position->second.getIntNumber();
//		int pos = mesh.at("primitives").array()[0].at("attributes").at("TEXCOORD_0").intNumber();
//		auto accessor = accessors.at(pos);
//		auto view = views.at(accessor.at("bufferView").intNumber());
//
//		BufferLayout layout;
//		layout.dataType = static_cast<ShaderDataType>(accessor.at("componentType").intNumber());
//		const std::string& type = accessor.at("type").string();
//
//		if (type == "VEC2")
//		{
//			layout.dataTypeCountPerVertex = 2;
//		}
//		else
//		{
//			ASSERT_NOT_REACHED();
//		}
//
//		layout.isNormalized = false;
//		layout.offset = view.at("byteOffset").intNumber() + accessor.at("byteOffset").intNumber();
//		layout.stride = view.at("byteStride").intNumber();
//		vao.bind();
//		buffers.at(view.at("buffer").intNumber()).bind();
//		vao.setAttribute(2, layout);
//		std::cout << layout.offset << ' ' << layout.stride << '\n';
//		std::cout << view.at("byteOffset").intNumber() << ' ' << accessor.at("byteOffset").intNumber() << '\n';
//	}
//
//	{
//		int indices = mesh.at("primitives").array()[0].at("indices").intNumber();
//
//		auto accessor = accessors.at(indices);
//		auto view = views[accessor.at("bufferView").intNumber()];
//		count = accessor.at("count").intNumber();
//		offset = view.at("byteOffset").intNumber() + accessor.at("byteOffset").intNumber();
//	}
//}
