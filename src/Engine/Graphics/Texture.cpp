#include <Engine/Graphics/Texture.hpp>
#include <stb_image.h>

Texture::Texture(std::string_view path)
{
	glGenTextures(1, &m_handle);

	glBindTexture(GL_TEXTURE_2D, m_handle);

	int width;
	int height;
	//stbi_load(path.data(), &width, &height)
}

Texture::~Texture()
{
	glDeleteTextures(1, &m_handle);
}
