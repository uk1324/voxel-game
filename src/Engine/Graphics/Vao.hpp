#pragma once

#include <Engine/Graphics/Vbo.hpp>
#include <glad/glad.h>

enum class ShaderDataType : uint32_t
{
	Byte = GL_BYTE,
	UnsignedByte = GL_UNSIGNED_BYTE,
	Short = GL_SHORT,
	UnsignedShort = GL_UNSIGNED_SHORT,
	Int = GL_INT,
	UnsignedInt = GL_UNSIGNED_INT,
	Float = GL_FLOAT,
};

struct BufferLayout
{
public:
	BufferLayout() = default;
	BufferLayout(ShaderDataType dataType, uint32_t dataTypeCountPerVertex, intptr_t offset, size_t stride, bool normalize);

public:
	ShaderDataType dataType;
	uint32_t dataTypeCountPerVertex;
	bool isNormalized;
	size_t stride;
	intptr_t offset;
};

struct IntBufferLayout
{
public:
	IntBufferLayout() = default;
	IntBufferLayout(ShaderDataType dataType, uint32_t dataTypeCountPerVertex, intptr_t offset, size_t stride);

public:
	ShaderDataType dataType;
	uint32_t dataTypeCountPerVertex;
	size_t stride;
	intptr_t offset;
};

class Vao
{
public:
	Vao();
	~Vao();

	Vao(const Vao&) = delete;
	Vao& operator= (const Vao&) = delete;

	Vao(Vao&& other) noexcept;
	Vao& operator= (Vao&& other) noexcept;

	uint32_t handle() const;

	// Sets the atribute of the currently bound VertexArray at index to the currently bound VertexBuffer with specified layout.
	static void setAttribute(uint32_t index, const BufferLayout& layout);
	static void setAttribute(uint32_t index, const IntBufferLayout& layout);

	static void setAttribute(
		uint32_t index,
		ShaderDataType dataType,
		uint32_t dataTypeCountPerVertex,
		bool normalize,
		size_t stride,
		intptr_t offset);

	static void setIntAttribute(
		uint32_t index,
		ShaderDataType dataType,
		uint32_t dataTypeCountPerVertex,
		size_t stride,
		intptr_t offset);

	void bind() const;

private:
	uint32_t m_handle;
};
