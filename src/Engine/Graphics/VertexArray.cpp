#include <Engine/Graphics/VertexArray.hpp>

#include <stddef.h>

VertexArray::VertexArray()
{
	glGenVertexArrays(1, &m_handle);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_handle);
}

VertexArray::VertexArray(VertexArray&& other) noexcept
	: m_handle(other.m_handle)
{
	other.m_handle = NULL;
}

VertexArray& VertexArray::operator=(VertexArray&& other) noexcept
{
	m_handle = other.m_handle;
	other.m_handle = NULL;
	return *this;
}

GLuint VertexArray::handle() const
{
	return m_handle;
}

//void VertexArray::setAttribute(GLuint index, GLint count)
//{
//	glVertexAttribPointer(index,
//}

void VertexArray::bind() const
{
	glBindVertexArray(m_handle);
}
