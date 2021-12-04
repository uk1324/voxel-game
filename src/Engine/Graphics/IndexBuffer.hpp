#pragma once

#include <stdint.h>

namespace Gfx
{
	class IndexBuffer
	{
	public:
		IndexBuffer();
		IndexBuffer(uint32_t* indices, size_t count);
		~IndexBuffer();

		IndexBuffer(const IndexBuffer&) = delete;
		IndexBuffer& operator= (const IndexBuffer&) = delete;

		IndexBuffer(IndexBuffer&& other) noexcept;
		IndexBuffer& operator= (IndexBuffer&& other) noexcept;

		void bind();
		void unbind();

	private:
		uint32_t m_handle;
	};
}