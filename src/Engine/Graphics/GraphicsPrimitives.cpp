#include <Engine/Graphics/GraphicsPrimitives.hpp>

static float cubeTriangleVertices[] = {
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,

    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f
};

std::unique_ptr<VertexArray> GraphicsPrimitives::cubeTrianglesVao;
VertexBuffer GraphicsPrimitives::cubeTrianglesVbo;

void GraphicsPrimitives::init()
{
	if (s_isInitialized == false)
	{
        cubeTrianglesVao = std::make_unique<VertexArray>();
        cubeTrianglesVao->bind();
        cubeTrianglesVbo = VertexBuffer(BufferBindTarget::ArrayBuffer, BufferUsage::StaticDraw, cubeTriangleVertices, sizeof(cubeTriangleVertices));
        cubeTrianglesVbo.bind(BufferBindTarget::ArrayBuffer);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, reinterpret_cast<void*>(0));
        glEnableVertexAttribArray(0);
	}
}

bool GraphicsPrimitives::s_isInitialized = false;