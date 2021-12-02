#include "Drawing.hpp"

ClearModeBit operator|(ClearModeBit a, ClearModeBit b)
{
	return static_cast<ClearModeBit>(static_cast<GLbitfield>(a) | static_cast<GLbitfield>(b));
}

void setClearColor(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
}

void clearBuffer(ClearModeBit mode)
{
	glClear(static_cast<GLbitfield>(mode));
}

void drawTriangles(size_t vertexCount)
{
	glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}

void drawLines(size_t pointCount)
{
	glDrawArrays(GL_LINES, 0, pointCount);
}
