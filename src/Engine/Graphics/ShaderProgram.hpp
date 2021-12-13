#pragma once

#include <Engine/Graphics/Shader.hpp>
#include <Math/Vec2.hpp>
#include <Math/Vec3.hpp>
#include <Math/Mat4.hpp>

#include <unordered_map>

// TODO: Find a good way to cache uniforms maybe inherit from ShaderProgram and store the uniforms location in the clas

namespace Gfx
{
	class ShaderProgram
	{
	public:
		ShaderProgram();
		ShaderProgram(std::string_view vertexPath, std::string_view fragmentPath);
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
		void setFloat(std::string_view name, float value);

		GLuint handle() const;

	private:
		// This isn't the best way to cache uniforms.
		// A better way would to to parse the glsl files and extract the variables declarations.
		// ^ (if you really tried you could make this solution also not require map lookup)
		// (doing this would be pretty hard because of struct declarations and other thing like iterface blocks).
		// An even faster solution would be to store the locations in variables so no hashmap lookup is required.
		// You could inherit from ShaderProgram and create a struct locations in the class and initialize them on creation.
		// That would require a lot of manual work and would be prone to errors so it isn't worth it.
		int getUniformLocation(std::string_view name);

	private:
		GLuint m_handle;

		std::unordered_map<std::string, int> m_cachedUniformLocations;
	};
}