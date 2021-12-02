#include <Engine/Graphics/TextureArray.hpp>
#include <Log/Log.hpp>
#include <stb_image.h>

#include <memory>

#include <iostream>

TextureArray::TextureArray(size_t width, size_t height, std::vector<std::string_view> textures)
{
	std::unique_ptr<uint32_t[]> data(new uint32_t[textures.size() * (width * height)]);

	for (size_t i = 0; i < textures.size(); i++)
	{
		int imageWidth, imageHeight, channelCount;
		static constexpr int RGBA_CHANNEL_COUNT = 4;
		uint32_t* imageData = reinterpret_cast<uint32_t*>(
			stbi_load(textures[i].data(), &imageWidth, &imageHeight, &channelCount, RGBA_CHANNEL_COUNT)
		);

		if ((imageWidth != width) || (imageHeight != height))
		{
			LOG_FATAL("array texture width doesn't match size (width: %d, height: %d)", imageWidth, imageHeight);
		}
		/*uint32_t* imageOffset = data.get() + (i * (width * height));*/
		uint32_t* imageOffset = data.get() + (i * (width * height));
		for (size_t i = 0; i < (width * height); i++)
		{
			imageOffset[i] = imageData[i];
		}
		stbi_image_free(imageData);
	}

	static constexpr GLsizei MIPMAP_LEVEL = 0;
	glGenTextures(1, &m_handle);
	bind();

	glTexImage3D(TARGET, MIPMAP_LEVEL, GL_RGBA, width, height, textures.size(), 0, GL_RGBA, GL_UNSIGNED_BYTE, data.get());

	//glTexStorage3D(TARGET, MIPMAP_LEVEL, GL_RGBA8, width, height, textures.size());

	glTexParameteri(TARGET, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(TARGET, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(TARGET, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(TARGET, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameterf(TARGET, GL_TEXTURE_MAX_LOD, 21000);
	//glTexParameterf(TARGET, GL_TEXTURE_MIN_LOD, -21000);

	glTexParameterf(TARGET, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16);

	glGenerateMipmap(TARGET);
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
