#include <Engine/Graphics/ShaderProgram.hpp>
#include <Log/Log.hpp>

ShaderProgram::ShaderProgram(std::vector<Shader> shaders)
	: m_handle(glCreateProgram())
{
	for (const Shader& shader : shaders)
	{
		glAttachShader(m_handle, shader.handle());
	}
	glLinkProgram(m_handle);

	GLint status;
	glGetProgramiv(m_handle, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		char infoLog[512];
		glGetProgramInfoLog(m_handle, sizeof(infoLog), nullptr, infoLog);
		LOG_FATAL("failed to link shaders: %s", infoLog);
	}
}

ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(m_handle);
}

void ShaderProgram::use()
{
	glUseProgram(m_handle);
}

void ShaderProgram::setVec2(std::string_view name, const Vec2& vec)
{
	glUniform2fv(glGetUniformLocation(m_handle, name.data()), 1, vec.data());
}

void ShaderProgram::setVec3(std::string_view name, const Vec3& vec)
{
	glUniform3fv(glGetUniformLocation(m_handle, name.data()), 1, vec.data());
}

GLuint ShaderProgram::handle() const
{
	return m_handle;
}
