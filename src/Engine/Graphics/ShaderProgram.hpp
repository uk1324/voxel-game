#pragma once

#include <Engine/Graphics/Shader.hpp>
#include <Math/Vec2.hpp>
#include <Math/Vec3.hpp>

#include <vector>

class ShaderProgram
{
public:
	ShaderProgram();
	~ShaderProgram();

	ShaderProgram(const ShaderProgram&) = delete;
	ShaderProgram& operator== (const ShaderProgram& other) = delete;

	ShaderProgram(ShaderProgram&&) noexcept;
	ShaderProgram& operator== (ShaderProgram&& other) noexcept;

	// Wanted to use the constructor to pass a vector of shaders but Shader is non copyable and you can't have a reference to a pointer.
	void addShader(const Shader& shader);
	void link();

	void use();

	void setVec2(std::string_view name, const Vec2& vec);
	void setVec3(std::string_view name, const Vec3& vec);

	GLuint handle() const;

private:
	GLuint m_handle;
};