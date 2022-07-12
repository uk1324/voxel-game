#pragma once

#include <Model/Model.hpp>
#include <Math/Quat.hpp>
#include <Utils/Opt.hpp>

#include <filesystem>
#include <string_view>
#include <vector>

// Maybe later add support for default texture, materials, other things if there is a resource manager class.

// Maybe add a method to Json::Value
// T at_or(Json::Value::StringType name, T defaultValue);

// This class could probably be converted to 1 pure function with lambdas inside.
class ModelLoader
{
private:
	struct BufferView
	{
		Vbo& buffer;
		size_t byteOffset;
		size_t byteLength;
		// If not specified data must be tightly packed. The data type information is inside the accessor.
		Opt<size_t> byteStride;
	};

	struct Accessor
	{
		BufferView& bufferView;
		size_t byteOffset;
		ShaderDataType componentType;
		Json::Value::StringType type;
		size_t count;
	};

public:
	ModelLoader(std::string_view path);

public:
	Model parse();

private:
	void loadBuffers();
	void loadBufferViews();
	void loadAccessors();
	void loadTextures();
	void loadMeshes();
	// A vao must be bound before calling.
	void loadVaoAttribute(size_t vaoIndex, Json::Value::IntType accessorIndex) const;
	static size_t sizeOfShaderDataType(ShaderDataType type);
	static size_t parseTypeCount(const Json::Value::StringType& type);
	void loadNodes();
	void loadSkins();
	void loadAnimations();

	template<typename T>
	T* bufferSlice(size_t bufferIndex, size_t byteOffset, size_t count) const;

public:
	static void propagateTransform(Model::Node& parent);

private:
	static Vec3 parseVec3(const Json::Value& vec);
	static Quat parseQuat(const Json::Value& quat);

private:
	Model model;

	std::vector<ByteBuffer> buffers;
	std::vector<BufferView> bufferViews;
	std::vector<Accessor> accessors;

	const Json::Value file;
	const std::filesystem::path directory;
};

template<typename T>
T* ModelLoader::bufferSlice(size_t bufferIndex, size_t byteOffset, size_t count) const
{
	const ByteBuffer& buffer = buffers[bufferIndex];
	if (byteOffset + count > buffer.size())
	{
		throw std::out_of_range();
	}
	return buffer.data() + byteOffset;
}
