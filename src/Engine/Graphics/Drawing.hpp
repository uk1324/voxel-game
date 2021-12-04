#pragma once

#include <glad/glad.h>

#include <stdint.h>

namespace Gfx
{
	enum class ClearModeBit : uint32_t
	{
		ColorBuffer = GL_COLOR_BUFFER_BIT,
		DepthBuffer = GL_DEPTH_BUFFER_BIT
	};

	void setClearColor(float r, float g, float b, float a);
	void clearBuffer(ClearModeBit mode);

	// Draws triangles of vertices offest to offset + vertexCount of the currently bound VertexArray.
	void drawTriangles(int32_t offset, uint32_t vertexCount);
	void drawLines(int32_t offset, uint32_t pointCount);
}

Gfx::ClearModeBit operator| (Gfx::ClearModeBit a, Gfx::ClearModeBit b);