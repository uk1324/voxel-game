#pragma once

#include <Engine/Graphics/VertexBuffer.hpp>

class VertexArray
{
public:
	VertexArray();
	~VertexArray();

	GLuint handle() const;

	void setAttribute();
	void bind() const;

private:
	GLuint m_handle;
};