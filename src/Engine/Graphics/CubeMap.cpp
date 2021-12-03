#include <Engine/Graphics/CubeMap.hpp>
#include <Log/Log.hpp>
#include <stb_image.h>

CubeMap::CubeMap()
	: m_handle(NULL)
{}

CubeMap::CubeMap(std::array<std::string_view, 6> texturePaths)
{
	glGenTextures(1, &m_handle);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_handle);

	for (int i = 0; i < int(texturePaths.size()); i++)
	{
		int width, height, channelCount;
		// Don't know if desired channels should be 0.
		uint8_t* data = stbi_load(texturePaths[i].data(), &width, &height, &channelCount, 0);
		if (data == nullptr)
		{
			LOG_FATAL("failed to load cubemap from %s", texturePaths[i].data());
		}
		// GL_TEXTURE_CUBE_MAP_***_* macros are sequential so I can just add i.
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		stbi_image_free(data);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

CubeMap::~CubeMap()
{
	glDeleteTextures(1, &m_handle);
}

void CubeMap::bind()
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_handle);
}
