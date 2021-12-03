#pragma

#include <glad/glad.h>

#include <array>
#include <string_view>

class CubeMap
{
public:
	CubeMap();
	CubeMap(std::array<std::string_view, 6> texturePaths);
	~CubeMap();

	void bind();

private:
	GLuint m_handle;
};