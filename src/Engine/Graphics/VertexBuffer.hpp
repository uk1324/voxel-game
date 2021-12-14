#pragma once

#include <stdint.h>

namespace Gfx
{
	class VertexBuffer
	{
	public:
		VertexBuffer();
		// Dynamic draw
		explicit VertexBuffer(size_t dataSize);
		// Static draw
		VertexBuffer(void* data, size_t dataSize);
		~VertexBuffer();

		VertexBuffer(const VertexBuffer&) = delete;
		VertexBuffer& operator= (const VertexBuffer&) = delete;

		VertexBuffer(VertexBuffer&& other) noexcept;
		VertexBuffer& operator= (VertexBuffer&& other) noexcept;

		// The VertexBuffer must be bound before calling.
		void setData(intptr_t offset, void* data, size_t dataSize);

		void bind() const;
		void bindAsIndexBuffer() const;

		const uint32_t handle() const;

	private:
		uint32_t m_handle;
	};
}