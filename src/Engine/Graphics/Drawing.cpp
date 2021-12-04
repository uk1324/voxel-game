#include <Engine/Graphics/Drawing.hpp>

using namespace Gfx;

void Gfx::setClearColor(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
}

void Gfx::clearBuffer(ClearModeBit mode)
{
	glClear(static_cast<GLbitfield>(mode));
}

void Gfx::drawTriangles(int32_t offset, uint32_t vertexCount)
{
	glDrawArrays(GL_TRIANGLES, offset, vertexCount);
}

void Gfx::drawLines(int32_t offset, uint32_t pointCount)
{
	glDrawArrays(GL_LINES, offset, pointCount);
}

ClearModeBit operator|(ClearModeBit a, ClearModeBit b)
{
	return static_cast<ClearModeBit>(static_cast<GLbitfield>(a) | static_cast<GLbitfield>(b));
}
