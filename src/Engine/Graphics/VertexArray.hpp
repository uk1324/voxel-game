#pragma once

#include <Engine/Graphics/VertexBuffer.hpp>

class VertexArray
{
public:
	VertexArray();
	~VertexArray();

	VertexArray(const VertexArray&) = delete;
	VertexArray& operator= (const VertexArray&) = delete;

	VertexArray(VertexArray&& other) noexcept;
	VertexArray& operator= (VertexArray&& other) noexcept;

	GLuint handle() const;

	void setAttribute();
	void bind() const;

private:
	GLuint m_handle;
};