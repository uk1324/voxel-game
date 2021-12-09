#pragma

#include <glad/glad.h>

#include <array>
#include <string_view>

namespace Gfx
{
	struct CubeMapTexturePaths
	{
	public:
		std::string_view right, left, top, bottom, front, back;
	};

	class CubeMap
	{
	public:
		CubeMap(const CubeMapTexturePaths& texture);
		CubeMap();
		~CubeMap();

		void bind();

	private:
		GLuint m_handle;
	};
}