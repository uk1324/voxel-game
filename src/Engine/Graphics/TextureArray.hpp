#pragma once

#include <glad/glad.h>

#include <vector>
#include <string_view>

// A texture array requires all textures to be the same size.

namespace Gfx
{
	class TextureArray
	{
	public:
		TextureArray(size_t width, size_t height, std::vector<std::string_view> textures);
		~TextureArray();

		TextureArray(const TextureArray&) = delete;
		TextureArray& operator= (const TextureArray&) = delete;

		TextureArray(TextureArray&& other) noexcept;
		TextureArray& operator= (TextureArray&& other) noexcept;

		GLuint handle() const;

		void bind() const;

	private:
		static constexpr GLenum TARGET = GL_TEXTURE_2D_ARRAY;

	private:
		GLuint m_handle;
	};
}