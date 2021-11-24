#pragma once

#include <glad/glad.h>

#include <string_view>

class Texture
{
public:
	Texture(std::string_view path);
	~Texture();

	Texture(const Texture&) = delete;
	Texture& operator= (const Texture&) = delete;

	Texture(Texture&& other) noexcept;
	Texture& operator= (Texture&& other) noexcept;

	void bind() const;

	GLuint handle() const;

private:
	static constexpr GLenum TARGET = GL_TEXTURE_2D;

private:
	GLuint m_handle;
};