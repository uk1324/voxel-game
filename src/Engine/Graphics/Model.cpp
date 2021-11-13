#include "Model.hpp"

Model::Model()
	: vao(NULL)
	, vbo(NULL)
{}

Model::Model(const Mesh& mesh)
{
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh.vertices.size() * 3, mesh.vertices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, reinterpret_cast<void*>(0));
	glEnableVertexAttribArray(0);
}

Model::~Model()
 {
	glDeleteVertexArrays(1, &vao);

	glDeleteBuffers(1, &vbo);
}

Model::Model(Model&& other) noexcept
	: vao(other.vao)
	, vbo(other.vbo)
{
	other.vao = NULL;
	other.vbo = NULL;
}

Model& Model::operator=(Model&& other) noexcept
{
	vao = other.vao;
	vbo = other.vbo;
	other.vao = NULL;
	other.vbo = NULL;
	return *this;
}
