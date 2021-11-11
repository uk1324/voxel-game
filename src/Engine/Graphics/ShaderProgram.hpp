#pragma once

#include <Engine/Graphics/Shader.hpp>

#include <vector>

class ShaderProgram
{
public:
	ShaderProgram(std::vector<Shader> shaders);
	~ShaderProgram();

	void use();

	GLuint handle() const;

private:
	GLuint m_handle;
};