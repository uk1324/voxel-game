#pragma once

#include <Engine/Graphics/Shader.hpp>
#include <Math/Vec2.hpp>
#include <Math/Vec3.hpp>
#include <Math/Mat4.hpp>

// TODO: Find a good way to cache uniforms maybe inherit from ShaderProgram and store the uniforms location in the clas

class ShaderProgram
{
public:
	ShaderProgram();
	~ShaderProgram();

	ShaderProgram(const ShaderProgram&) = delete;
	ShaderProgram& operator== (const ShaderProgram& other) = delete;

	ShaderProgram(ShaderProgram&&) noexcept;
	ShaderProgram& operator== (ShaderProgram&& other) noexcept;

	// Wanted to use the constructor to pass a vector of shaders but Shader is non copyable and you can't have a pointer to a reference.
	void addShader(const Shader& shader);
	void link();

	void use();

	void setVec2(std::string_view name, const Vec2& vec);
	void setVec3(std::string_view name, const Vec3& vec);
	void setVec3I(std::string_view name, const Vec3I& vec);
	void setMat4(std::string_view name, const Mat4& mat);
	void setInt(std::string_view name, int value);

	GLuint handle() const;

private:
	GLuint m_handle;
};