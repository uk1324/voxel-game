#include <Engine/Graphics/VertexBuffer.hpp>

#include <stddef.h>

VertexBuffer::VertexBuffer()
	: m_handle(NULL)
{}

VertexBuffer::VertexBuffer(BufferBindTarget target, BufferUsage usage, void* data, GLsizeiptr dataSize)
{
	glGenBuffers(1, &m_handle);
	bind(target);
	glBufferData(static_cast<GLenum>(target), dataSize, data, static_cast<GLenum>(usage));
}

VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &m_handle);
}

VertexBuffer::VertexBuffer(VertexBuffer&& other) noexcept
	: m_handle(other.m_handle)
{
	other.m_handle = NULL;
}

VertexBuffer& VertexBuffer::operator=(VertexBuffer&& other) noexcept
{
	m_handle = other.m_handle;
	other.m_handle = NULL;
	return *this;
}

void VertexBuffer::bind(BufferBindTarget target) const
{
	glBindBuffer(static_cast<GLenum>(target), m_handle);
}

const GLuint VertexBuffer::handle() const
{
	return m_handle;
}
