#pragma once

#include <Engine/Graphics/VertexBuffer.hpp>
#include <glad/glad.h>

namespace Gfx
{
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
		BufferLayout(ShaderDataType dataType, uint32_t dataTypeCountPerVertex, intptr_t offset, size_t stride, bool isNormalized);

	public:
		ShaderDataType dataType;
		uint32_t dataTypeCountPerVertex;
		bool isNormalized;
		size_t stride;
		intptr_t offset;
	};

	class VertexArray
	{
	public:
		VertexArray();
		~VertexArray();

		VertexArray(const VertexArray&) = delete;
		VertexArray& operator= (const VertexArray&) = delete;

		VertexArray(VertexArray&& other) noexcept;
		VertexArray& operator= (VertexArray&& other) noexcept;

		uint32_t handle() const;

		// Sets the atribute at index to the currently bound VertexBuffer with specified layout.
		// The VertexArray must be bound before calling.
		void setAttribute(uint32_t index, const BufferLayout& layout);

		void bind() const;

	private:
		uint32_t m_handle;
	};
}