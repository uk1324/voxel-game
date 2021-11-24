#pragma once

#include <glad/glad.h>

// Maybe put this into namespace

enum class ClearModeBit : GLbitfield
{
	ColorBuffer = GL_COLOR_BUFFER_BIT,
	DepthBuffer = GL_DEPTH_BUFFER_BIT
};

ClearModeBit operator| (ClearModeBit a, ClearModeBit b);

void setClearColor(float r, float g, float b, float a);
void clearBuffer(ClearModeBit mode);

void drawTriangles(size_t vertexCount);