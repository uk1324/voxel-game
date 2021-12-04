#pragma once

#include <Engine/Graphics/VertexArray.hpp>

#include <memory>

namespace Gfx
{
	class Primitives
	{
	public:
		// Should be called after OpenGL is initialized.
		static void init();

	public:
		// Have to use a pointer because OpenGL is not initialized yet.
		static std::unique_ptr<VertexArray> cubeTrianglesVao;

	private:
		static std::unique_ptr<VertexBuffer> cubeTrianglesVbo;
		static bool s_isInitialized;
	};
}