#pragma once

#include <stdint.h>

class Vbo
{
public:
	Vbo();
	// Dynamic draw
	explicit Vbo(size_t dataByteSize);
	// Static draw
	Vbo(const void* data, size_t dataByteSize);
	~Vbo();

	static Vbo generate();

	Vbo(const Vbo&) = delete;
	Vbo& operator= (const Vbo&) = delete;

	Vbo(Vbo&& other) noexcept;
	Vbo& operator= (Vbo&& other) noexcept;

	// The VertexBuffer must be bound before calling.
	static void setData(intptr_t offset, const void* data, size_t dataByteSize);
	static void allocateData(const void* data, size_t dataByteSize);

	void bind() const;
	void bindAsIndexBuffer() const;

	const uint32_t handle() const;

private:
	uint32_t m_handle;
};
