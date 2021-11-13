#pragma once

#include <Engine/Graphics/Mesh.hpp>
#include <glad/glad.h>

class Model
{
public:
	Model();
	Model(const Mesh& mesh);
	~Model();

	Model(Model&&) noexcept;
	Model& operator= (Model&&) noexcept;

	Model(const Model& other) = delete;
	Model& operator= (const Model& other)= delete;

public:
	GLuint vao;
	GLuint vbo;
};