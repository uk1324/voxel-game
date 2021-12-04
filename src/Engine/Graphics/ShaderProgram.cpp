#include <Engine/Graphics/ShaderProgram.hpp>
#include <Log/Log.hpp>

using namespace Gfx;

ShaderProgram::ShaderProgram()
	: m_handle(glCreateProgram())
{
}

ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(m_handle);
}

ShaderProgram::ShaderProgram(ShaderProgram&& other) noexcept
	: m_handle(other.m_handle)
{
	other.m_handle = NULL;
}

ShaderProgram& ShaderProgram::operator==(ShaderProgram&& other) noexcept
{
	glDeleteProgram(m_handle);
	m_handle = other.m_handle;
	other.m_handle = NULL;
	return *this;
}

void ShaderProgram::addShader(const Shader& shader)
{
	glAttachShader(m_handle, shader.handle());
}

void ShaderProgram::link()
{
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

void ShaderProgram::use()
{
	glUseProgram(m_handle);
}

// Using glProgramUniform instead of glUniform might be sometimes faster because it doesn't need a glUsePgoram call before it.
void ShaderProgram::setVec2(std::string_view name, const Vec2& vec)
{
	glProgramUniform2fv(m_handle, glGetUniformLocation(m_handle, name.data()), 1, vec.data());
}

void ShaderProgram::setVec3(std::string_view name, const Vec3& vec)
{
	glProgramUniform3fv(m_handle, glGetUniformLocation(m_handle, name.data()), 1, vec.data());
}

void ShaderProgram::setVec3I(std::string_view name, const Vec3I& vec)
{
	glProgramUniform3iv(m_handle, glGetUniformLocation(m_handle, name.data()), 1, vec.data());
}

void ShaderProgram::setMat4(std::string_view name, const Mat4& mat)
{
	glProgramUniformMatrix4fv(m_handle, glGetUniformLocation(m_handle, name.data()), 1, GL_FALSE, mat.data());
}

void ShaderProgram::setInt(std::string_view name, int value)
{
	glProgramUniform1i(m_handle, glGetUniformLocation(m_handle, name.data()), value);
}

GLuint ShaderProgram::handle() const
{
	return m_handle;
}

int ShaderProgram::getUniformLocation(std::string_view name)
{
	// Can't assume that the string_view data won't get destroyed.
	std::string uniformName = std::string(name);
	auto location = m_cachedUniformLocations.find(uniformName);
	if (location == m_cachedUniformLocations.end())
	{
		int location = glGetUniformLocation(m_handle, uniformName.c_str());
		m_cachedUniformLocations[std::move(uniformName)] = location;
	}
	return location->second;
}
