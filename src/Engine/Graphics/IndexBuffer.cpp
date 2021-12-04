#include <Engine/Graphics/IndexBuffer.hpp>
#include <glad/glad.h>

#include <stddef.h>

using namespace Gfx;

IndexBuffer::IndexBuffer()
	: m_handle(NULL)
{}

IndexBuffer::IndexBuffer(uint32_t* indices, size_t count)
{
	glGenBuffers(1, &m_handle);
	// GL_ELEMENT_ARRAY_BUFFER is not valid without an actively bound vao.
	// Binding with GL_ARRAY_BUFFER allows the data to be loaded regardless of vao state. 
	glBindBuffer(GL_ARRAY_BUFFER, m_handle);
	glBufferData(GL_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer()
{
	glDeleteBuffers(1, &m_handle);
}

IndexBuffer::IndexBuffer(IndexBuffer&& other) noexcept
	: m_handle(other.m_handle)
{
	other.m_handle = NULL;
}

IndexBuffer& IndexBuffer::operator=(IndexBuffer&& other) noexcept
{
	glDeleteBuffers(1, &m_handle);
	m_handle = other.m_handle;
	other.m_handle = NULL;
	return *this;
}

void IndexBuffer::bind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_handle);
}

void IndexBuffer::unbind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
