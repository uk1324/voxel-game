#pragma once

#include <Engine/Ecs/EntityManager.hpp>
#include <Engine/Graphics/GraphicsPrimitives.hpp>
#include <Engine/Graphics/ShaderProgram.hpp>
#include <Model/Model.hpp>

struct RenderingComponent
{

};

class RenderingSystem
{
public:
	RenderingSystem();

	void update(const EntityManager& entityManager, const Mat4& view, const Mat4& projection);

private:
	Gfx::ShaderProgram m_shader;
	Model m_model;
};