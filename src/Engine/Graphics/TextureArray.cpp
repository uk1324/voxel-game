#include <Engine/Graphics/TextureArray.hpp>
#include <Log/Log.hpp>
#include <stb_image.h>

#include <memory>

TextureArray::TextureArray(size_t width, size_t height, std::vector<std::string_view> textures)
{
	glGenTextures(1, &m_handle);
	bind();
	std::unique_ptr<uint32_t[]> data(new uint32_t[width * height]);

	for (size_t i = 0; i < textures.size(); i++)
	{
		int imageWidth, imageHeight, channelCount;
		static constexpr int RGBA_CHANNEL_COUNT = 4;
		stbi_load(textures[i].data(), &imageWidth, &imageHeight, &channelCount, RGBA_CHANNEL_COUNT);
		if ((imageWidth != width) || (imageHeight != height))
		{
			LOG_FATAL("array texture width doesn't match (width: %d, height: %d)", imageWidth, imageHeight);
		}
	}
}

TextureArray::~TextureArray()
{
	glDeleteTextures(1, &m_handle);
}

TextureArray::TextureArray(TextureArray&& other) noexcept
	: m_handle(other.m_handle)
{
	other.m_handle = NULL;
}

TextureArray& TextureArray::operator=(TextureArray&& other) noexcept
{
	m_handle = other.m_handle;
	other.m_handle = NULL;
	return *this;
}

GLuint TextureArray::handle() const
{
	return m_handle;
}

void TextureArray::bind() const
{
	glBindTexture(TARGET, m_handle);
}
