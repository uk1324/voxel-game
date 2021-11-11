#pragma once

#include <glad/glad.h>

#include <string_view>

enum class ShaderType
{
	Vertex = GL_VERTEX_SHADER,
	Fragment = GL_FRAGMENT_SHADER,
	Geometry = GL_GEOMETRY_SHADER,
};

class Shader
{
public:
	Shader(std::string_view path, ShaderType type);
	~Shader();

	GLuint handle() const;

private:
	GLuint m_handle;
};