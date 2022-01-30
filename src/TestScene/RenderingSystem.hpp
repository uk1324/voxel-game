#pragma once

#include <Engine/Ecs/EntityManager.hpp>
#include <Engine/Graphics/ShaderProgram.hpp>
#include <Model/Model.hpp>

struct RenderingComponent
{

};

class RenderSystem
{
public:
	RenderSystem();

	void update(const EntityManager& entityManager, Entity player, const Mat4& view, const Mat4& projection);

private:
	ShaderProgram m_shader;
	Model m_model;
};