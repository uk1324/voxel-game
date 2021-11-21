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

	Texture(Texture&&) noexcept;
	Texture& operator= (Texture&&) noexcept;

private:
	GLuint m_handle;
};