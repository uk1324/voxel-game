#include <Engine/Graphics/VertexArray.hpp>

VertexArray::VertexArray()
{
	glGenVertexArrays(1, &m_handle);
}

VertexArray::~VertexArray()
{
	glDeleteBuffers(1, &m_handle);
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
