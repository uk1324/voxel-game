#include "Shader.hpp"

#include <Utils/FileIo.hpp>
#include <Log/Log.hpp>

Shader::Shader(std::string_view path, ShaderType type)
{
	m_handle = glCreateShader(static_cast<GLenum>(type));

	std::string source = stringFromFile(path);
	const char* src = source.c_str();
	GLint length = source.length();
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
