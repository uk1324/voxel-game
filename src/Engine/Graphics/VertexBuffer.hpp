#pragma once

#include <glad/glad.h>

enum class BufferBindTarget
{
	ArrayBuffer = GL_ARRAY_BUFFER
};

enum class BufferUsage
{
	StaticDraw = GL_STATIC_DRAW,
	DynamicDraw = GL_DYNAMIC_DRAW
};

enum class BufferDataType
{
	Float = GL_FLOAT,
	UnsignedInt = GL_UNSIGNED_INT,
	Int = GL_INT
};

class VertexBuffer
{
public:
	VertexBuffer();
	VertexBuffer(BufferBindTarget target, BufferUsage usage, void* data, GLsizeiptr dataSize);
	~VertexBuffer();

	VertexBuffer(const VertexBuffer&) = delete;
	VertexBuffer& operator= (const VertexBuffer&) = delete;

	VertexBuffer(VertexBuffer&& other) noexcept;
	VertexBuffer& operator= (VertexBuffer&& other) noexcept;

	void bind(BufferBindTarget target) const;

	const GLuint handle() const;

private:
	GLuint m_handle;
};