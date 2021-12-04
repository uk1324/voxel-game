#include <Engine/Graphics/Shader.hpp>
#include <Utils/FileIo.hpp>
#include <Log/Log.hpp>

using namespace Gfx;

Shader::Shader(std::string_view path, ShaderType type)
{
	m_handle = glCreateShader(static_cast<GLenum>(type));

	std::string source = stringFromFile(path);
	const char* src = source.c_str();
	GLint length = static_cast<GLint>(source.length());
	glShaderSource(m_handle, 1, &src, &length);
	glCompileShader(m_handle);
	GLint status;
	glGetShaderiv(m_handle, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		char infoLog[512];
		glGetShaderInfoLog(m_handle, sizeof(infoLog), nullptr, infoLog);
		LOG_FATAL("failed to compile shader %s\n%s\n", path.data(), infoLog);
	}
}

Shader::Shader(Shader&& other) noexcept
	: m_handle(other.m_handle)
{
	other.m_handle = NULL;
}

Shader& Shader::operator=(Shader&& other) noexcept
{
	glDeleteShader(m_handle);
	m_handle = other.m_handle;
	other.m_handle = NULL;
	return *this;
}

Shader::~Shader()
{
	glDeleteShader(m_handle);
}

GLuint Shader::handle() const
{
	return m_handle;
}
