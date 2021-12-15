#include <Engine/Graphics/VertexArray.hpp>

#include <stddef.h>

using namespace Gfx;

BufferLayout::BufferLayout(ShaderDataType dataType, uint32_t dataTypeCountPerVertex, intptr_t offset, size_t stride, bool isNormalized)
	: dataType(dataType)
	, dataTypeCountPerVertex(dataTypeCountPerVertex)
	, offset(offset)
	, stride(stride)
	, isNormalized(isNormalized)
{}

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
	glDeleteVertexArrays(1, &m_handle);
	m_handle = other.m_handle;
	other.m_handle = NULL;
	return *this;
}

uint32_t VertexArray::handle() const
{
	return m_handle;
}

void VertexArray::setAttribute(uint32_t index, const BufferLayout& layout)
{
	glVertexAttribPointer(index,
		layout.dataTypeCountPerVertex,
		static_cast<GLenum>(layout.dataType),
		layout.isNormalized,
		layout.stride, 
		reinterpret_cast<void*>(layout.offset)
	);
	glEnableVertexAttribArray(index);
}

void VertexArray::bind() const
{
	glBindVertexArray(m_handle);
}
