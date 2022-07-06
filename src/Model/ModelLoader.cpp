#include <Model/ModelLoader.hpp>
#include <Math/Quat.hpp>
#include <Log/Log.hpp>
#include <Image/Image32.hpp>

ModelLoader::ModelLoader(std::string_view path)
	: file(jsonFromFile(path))
	, directory(std::filesystem::path(path).parent_path())
{}

Model ModelLoader::parse()
{
	try
	{
		loadBuffers();
		loadBufferViews();
		loadAccessors();
		loadTextures();
		loadMeshes();
		loadNodes();
		loadAnimations();
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

	return std::move(model);
}

void ModelLoader::loadBuffers()
{
	for (const auto& buffer : file.at("buffers").array())
	{
		ByteBuffer data = byteBufferFromFile((directory / buffer.at("uri").string()).string());
		
		auto size = buffer.at("byteLength").intNumber();
		if (data.size() != size)
			LOG_FATAL("buffer data length doesn't match the specified length");

		model.buffers.push_back(Vbo(data.data(), data.size()));
		buffers.push_back(std::move(data));
	}
}

void ModelLoader::loadBufferViews()
{
	for (const auto& bufferView : file.at("bufferViews").array())
	{
		Vbo& buffer = model.buffers.at(bufferView.at("buffer").intNumber());
		size_t byteOffset = bufferView.contains("byteOffset") ? bufferView.at("byteOffset").intNumber() : 0;
		size_t byteLength = bufferView.at("byteLength").intNumber();
		auto byteStride = bufferView.contains("byteStride") ? Opt<size_t>(bufferView.at("byteStride").intNumber()) : std::nullopt;
		bufferViews.push_back(BufferView{ buffer, byteOffset, byteLength, byteStride });
	}
}

void ModelLoader::loadAccessors()
{
	for (const auto& accessor : file.at("accessors").array())
	{
		BufferView& bufferView = bufferViews.at(accessor.at("bufferView").intNumber());
		size_t byteOffset = accessor.contains("byteOffset") ? accessor.at("byteOffset").intNumber() : 0;
		// TODO: Maybe later check if it is a valid componentType.
		auto componentType = static_cast<ShaderDataType>(accessor.at("componentType").intNumber());
		const Json::Value::StringType& type = accessor.at("type").string();
		size_t count = accessor.at("count").intNumber();
		accessors.push_back(Accessor{ bufferView, byteOffset, componentType, type, count });
	}
}

void ModelLoader::loadTextures()
{
	const auto& images = file.at("images").array();
	const auto& samplers = file.contains("samplers") ? file.at("samplers") : Json::Value::emptyArray();
	for (const auto& texture : file.at("textures").array())
	{
		auto textureData = Texture::generate();
		textureData.bind();

		// TODO: Later check if the magFilter, minFilter, wrapS, wrapT are valid.
		if (texture.contains("sampler"))
		{
			const auto& sampler = samplers.at(texture.at("sampler").intNumber());
			if (sampler.contains("magFilter"))
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, sampler.at("magFilter").intNumber());
			}
			if (sampler.contains("minFilter"))
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, sampler.at("minFilter").intNumber());
			}
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, sampler.contains("wrapS") ? sampler.at("wrapS").intNumber() : GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, sampler.contains("wrapT") ? sampler.at("wrapT").intNumber() : GL_REPEAT);
		}
		else
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		}

		// Default textures are not supported.
		const auto& image = images.at(texture.at("source").intNumber());
		const Image32 imageData((directory / image.at("uri").string()).string().c_str());
		
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageData.width(), imageData.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData.data());
		glGenerateMipmap(GL_TEXTURE_2D);

		model.textures.push_back(std::move(textureData));
	}
}

void ModelLoader::loadMeshes()
{
	const auto& materials = file.at("materials").array();
	for (const auto& mesh : file.at("meshes").array())
	{
		// Currently only a single primivies is supported.
		const auto& primitives = mesh.at("primitives").at(0);
		const auto& attributes = primitives.at("attributes");

		// No default material supported.
		const auto& material = materials.at(primitives.at("material").intNumber());
		Texture& texture = model.textures.at(material.at("pbrMetallicRoughness").at("baseColorTexture").at("index").intNumber());

		Vao vao;
		vao.bind();
		// Position always has min max properites defined so I can create some bounding shape for frustrum culling.
		loadVaoAttribute(0, attributes.at("POSITION").intNumber());
		loadVaoAttribute(1, attributes.at("NORMAL").intNumber());
		loadVaoAttribute(2, attributes.at("TEXCOORD_0").intNumber());
 		loadVaoAttribute(3, attributes.at("JOINTS_0").intNumber());
		loadVaoAttribute(4, attributes.at("WEIGHTS_0").intNumber());

		const Accessor& indicesAccessor = accessors.at(primitives.at("indices").intNumber());
		const BufferView& indicesBufferView = indicesAccessor.bufferView;

		model.meshes.push_back(Model::Mesh{
			std::move(vao),
			texture,
			indicesBufferView.buffer,
			indicesBufferView.byteOffset + indicesAccessor.byteOffset,
			indicesAccessor.count,
			indicesAccessor.componentType
		});
	}
}

void ModelLoader::loadVaoAttribute(size_t vaoIndex, Json::Value::IntType accessorIndex) const
{
	const Accessor& accessor = accessors.at(accessorIndex);
	const BufferView& bufferView = accessor.bufferView;

	size_t byteOffset = bufferView.byteOffset + accessor.byteOffset;

	size_t componentTypePreVertexCount = parseTypeCount(accessor.type);
	size_t componentTypeSize = sizeOfShaderDataType(accessor.componentType);
	size_t byteStride = bufferView.byteStride.value_or(componentTypePreVertexCount * componentTypeSize);

	const Vbo& vbo = bufferView.buffer;
	vbo.bind();
	Vao::setAttribute(vaoIndex, accessor.componentType, componentTypePreVertexCount, false, byteStride, byteOffset);
}

size_t ModelLoader::sizeOfShaderDataType(ShaderDataType type)
{
	switch (type)
	{
		case ShaderDataType::Float: return 4;
		case ShaderDataType::UnsignedShort: return 2;
	}

	LOG_FATAL("invalid shader data type");
}

size_t ModelLoader::parseTypeCount(const Json::Value::StringType& type)
{
	if (type == "SCALAR")
		return 1;
	if (type == "VEC2")
		return 2;
	if (type == "VEC3")
		return 3;
	if (type == "VEC4")
		return 4;

	LOG_FATAL("invalid type");
}

#include <Math/Angles.hpp>
#include <iostream>

void ModelLoader::loadNodes()
{
	model.nodes.resize(file.at("nodes").array().size());
	
	const auto& nodes = file.at("nodes").array();
	for (size_t i = 0; i < nodes.size(); i++)
	{
		const auto& node = nodes[i];

		const Vec3 scale = node.contains("scale") ? parseVec3(node.at("scale")) : Vec3(1);
		const Quat rotation = node.contains("rotation") ? parseQuat(node.at("rotation")) : Quat::identity;
		const Vec3 translation = node.contains("translation") ? parseVec3(node.at("translation")) : Vec3(0);
		model.nodes[i].transform = Mat4::scale(scale) * rotation.asMatrix() * Mat4::translation(translation);

		if (node.contains("children"))
		{
			for (const auto& child : node.at("children").array())
			{
				model.nodes[i].children.push_back(&model.nodes.at(child.intNumber()));
			}
		}

		if (node.contains("skin"))
		{
			const auto& skin = file.at("skins").at(node.at("skin").intNumber());

			const Accessor& accessor = accessors.at(skin.at("inverseBindMatrices").intNumber());
			const BufferView& bufferView = accessor.bufferView;
			std::string data = stringFromFile((directory / file.at("buffers").at(0).at("uri").string()).string());
			size_t byteOffset = bufferView.byteOffset + accessor.byteOffset;
			model.inverseBindMatrices.resize(skin.at("joints").array().size());
			memcpy(model.inverseBindMatrices.data(), data.data() + byteOffset, sizeof(Mat4) * skin.at("joints").array().size());

			for (const auto& jointIndex : skin.at("joints").array())
			{
				model.joints.push_back(&model.nodes.at(jointIndex.intNumber())); 
			}
		}
	}

	for (const auto& node : file.at("scenes").at(0).at("nodes").array())
	{
		model.children.push_back(&model.nodes.at(node.intNumber()));
		//propagateTransform(model.nodes.at(node.intNumber()));
	}

}

// Make a function that takes an accessor and returs a ptr to the data.
void ModelLoader::loadAnimations()
{
	std::string data = stringFromFile((directory / file.at("buffers").at(0).at("uri").string()).string());

	for (const auto& animation : file.at("animations").array())
	{
		const auto& samplers = animation.at("samplers").array();
		for (const auto& channel : animation.at("channels").array())
		{
			const auto& sampler = samplers.at(channel.at("sampler").intNumber());
			const Accessor& accessor = accessors.at(sampler.at("input").intNumber());
			// ASSERT scalar and float

			auto keyFrames = reinterpret_cast<float*>(data.data() + accessor.byteOffset + accessor.bufferView.byteOffset);
			auto keyFramesEnd = keyFrames + accessor.count;

			model.keyframes.resize(accessor.count);
			for (auto& keyframe : model.keyframes)
			{
				// TODO find a way to resize without default initializing.
				// Could use std::unique_ptr
				keyframe.rotation.resize(model.nodes.size());
				keyframe.translation.resize(model.nodes.size());
				keyframe.scale.resize(model.nodes.size());
			}

			const auto& target = channel.at("target");

			size_t nodeIndex = target.at("node").intNumber();
			const Accessor& a = accessors.at(sampler.at("output").intNumber());
			ASSERT(a.count == accessor.count);
			if (target.at("path").string() == "translation")
			{
				auto transformations = reinterpret_cast<Vec3*>(data.data() + a.byteOffset + a.bufferView.byteOffset);
				for (size_t i = 0; i < a.count; i++)
				{
					model.keyframes[i].time = keyFrames[i];
					model.keyframes[i].translation[nodeIndex] = transformations[i];
				}
			}

			if (target.at("path").string() == "scale")
			{
				auto transformations = reinterpret_cast<Vec3*>(data.data() + a.byteOffset + a.bufferView.byteOffset);
				for (size_t i = 0; i < a.count; i++)
				{
					model.keyframes[i].time = keyFrames[i];
					model.keyframes[i].scale[nodeIndex] = transformations[i];
				}
			}

			if (target.at("path").string() == "rotation")
			{
				auto transformations = reinterpret_cast<Quat*>(data.data() + a.byteOffset + a.bufferView.byteOffset);
				for (size_t i = 0; i < a.count; i++)
				{
					model.keyframes[i].time = keyFrames[i];
					model.keyframes[i].rotation[nodeIndex] = transformations[i];
				}
			}
		}
	}
}

void ModelLoader::propagateTransform(Model::Node& parent)
{
	for (auto& child : parent.children)
	{
		//child->transform *= parent.transform;
		/*child->output = child->transform * child->output * parent.output;*/
		child->output = child->output * parent.output;
		/*child->output = child->output * parent.output * child->transform;*/
		/*child->output = child->output * child->transform * parent.output;*/
		//child->output = child->transform * parent.output * child->output;
		//child->output = parent.output * child->transform * child->output;
		//child->output = parent.output * child->output * child->transform;
		propagateTransform(*child);
	}
}

Vec3 ModelLoader::parseVec3(const Json::Value& vec)
{
	const auto& v = vec.array();
	return Vec3(v.at(0).number(), v.at(1).number(), v.at(2).number());
}

Quat ModelLoader::parseQuat(const Json::Value& quat)
{
	const auto& q = quat.array();
	return Quat(q.at(0).number(), q.at(1).number(), q.at(2).number(), q.at(3).number());
}
