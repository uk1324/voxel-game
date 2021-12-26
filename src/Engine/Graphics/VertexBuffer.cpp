#include <Engine/Graphics/VertexBuffer.hpp>

#include <glad/glad.h>

using namespace Gfx;

VertexBuffer::VertexBuffer()
	: m_handle(NULL)
{}

VertexBuffer::VertexBuffer(size_t dataByteSize)
{
	glGenBuffers(1, &m_handle);
	bind();
	glBufferData(GL_ARRAY_BUFFER, dataByteSize, nullptr, GL_DYNAMIC_DRAW);
}

VertexBuffer::VertexBuffer(const void* data, size_t dataByteSize)
{
	glGenBuffers(1, &m_handle);
	bind();
	glBufferData(GL_ARRAY_BUFFER, dataByteSize, data, GL_STATIC_DRAW);
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
	glDeleteBuffers(1, &m_handle);
	m_handle = other.m_handle;
	other.m_handle = NULL;
	return *this;
}

void VertexBuffer::setData(intptr_t offset, const void* data, size_t dataSize)
{
	glBufferSubData(GL_ARRAY_BUFFER, offset, dataSize, data);
}

void VertexBuffer::bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_handle);
}

void VertexBuffer::bindAsIndexBuffer() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_handle);
}

const GLuint VertexBuffer::handle() const
{
	return m_handle;
}
