#pragma once

#include <Model/Model.hpp>
#include <Math/Quat.hpp>
#include <Utils/Opt.hpp>

#include <filesystem>
#include <string_view>
#include <vector>

// Don't know if it is better to use pointers or indices. I won't be modyfing the vectors but if I do the pointers might get invalidated.
// When using indices the compiler has to do one
// lea indexRegister, [index+index*2]
// sal indexRegister, 5 and then lea;
// lea returnRegister, [arrayPtrRegister+indexRegister]
// It basically does (index * 3) << 5 Which is the same as index * 96 because 96 is the size of the struct.
// I don't know how much faster that is. The compiler probably can't optimize the array accesses away because
// in skeletal animation I won't just be using a loop to iterate over the elemnts I will also need things like the parent
// transform which would probably require more jumping around in memory.
// Using pointers also makes the models non copyable but also contain opengl obejcts which also make them non copyable.

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
		throw std::out_of_range("");
	}
	return buffer.data() + byteOffset;
}
