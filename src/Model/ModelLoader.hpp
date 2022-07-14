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
		ByteBuffer& data;
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

	template<typename T>
	class Buffer
	{
	public:
		Buffer(void* data, size_t stride, size_t count);
		const T& operator[](size_t index) const;

		size_t count() const;

	private:
		char* m_data;
		size_t m_stride;
		size_t m_count;
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
	T* bufferSlice(size_t bufferAccessorIndex, size_t byteOffset, size_t count) const;
	template<typename T>
	Buffer<T> createBuffer(size_t bufferViewIndex);

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

template<typename T>
ModelLoader::Buffer<T> ModelLoader::createBuffer(size_t bufferAccessorIndex)
{
	const Accessor& accessor = accessors.at(bufferAccessorIndex);
	const BufferView& bufferView = accessor.bufferView;

	const auto byteOffset = bufferView.byteOffset + accessor.byteOffset;

	const auto componentTypePreVertexCount = parseTypeCount(accessor.type);
	const auto componentTypeSize = sizeOfShaderDataType(accessor.componentType);
	const auto byteStride = bufferView.byteStride.value_or(componentTypePreVertexCount * componentTypeSize);

	return Buffer<T>(bufferView.data.data() + byteOffset, byteStride, accessor.count);
}

template<typename T>
ModelLoader::Buffer<T>::Buffer(void* data, size_t stride, size_t count)
	: m_data(reinterpret_cast<char*>(data))
	, m_stride(stride)
	, m_count(count)
{
	ASSERT(m_stride > alignof(T));
}

template<typename T>
const T& ModelLoader::Buffer<T>::operator[](size_t index) const
{
	return m_data[index * m_stride];
}

template<typename T>
size_t ModelLoader::Buffer<T>::count() const
{
	return m_count;
}
