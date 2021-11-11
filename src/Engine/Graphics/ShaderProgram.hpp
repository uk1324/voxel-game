#pragma once

#include <Engine/Graphics/Shader.hpp>
#include <Math/Vec2.hpp>
#include <Math/Vec3.hpp>

#include <vector>

class ShaderProgram
{
public:
	ShaderProgram(std::vector<Shader> shaders);
	~ShaderProgram();

	void use();

	void setVec2(std::string_view name, const Vec2& vec);
	void setVec3(std::string_view name, const Vec3& vec);

	GLuint handle() const;

private:
	GLuint m_handle;
};