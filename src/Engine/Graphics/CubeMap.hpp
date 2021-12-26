#pragma

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
		uint32_t m_handle;
	};
}