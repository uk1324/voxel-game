#pragma once

#include <Model/Model.hpp>
#include <Math/Quat.hpp>
#include <Utils/Opt.hpp>

#include <filesystem>
#include <string_view>
#include <vector>

// Don't know if it is better to use pointers or indices. I won't be modyfing the vectors but if I do the pointers might get invalidated.

// Maybe later add support for default texture, materials, other things if there is a resource manager class.

// Maybe add a method to Json::Value
// T at_or(Json::Value::StringType name, T defaultValue);
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

private:
	static void propagateTransform(Model::Node& parent);
	static Vec3 parseVec3(const Json::Value& vec);
	static Quat parseQuat(const Json::Value& quat);

private:
	Model model;

	std::vector<BufferView> bufferViews;
	std::vector<Accessor> accessors;

	const Json::Value file;
	const std::filesystem::path directory;
};