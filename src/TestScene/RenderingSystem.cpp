#include <TestScene/RenderingSystem.hpp>
#include <Engine/Graphics/Drawing.hpp>
#include <Game/Components/Position.hpp>
#include <Game/Components/Rotation.hpp>

RenderingSystem::RenderingSystem()
	: m_shader("src/TestScene/shader.vert", "src/TestScene/shader.frag")
{
	
}

#include <iostream>
#include <Engine/Time.hpp>

void RenderingSystem::update(const EntityManager& entityManager, const Mat4& view, const Mat4& projection)
{
	// Can remove clear ColorBuffer if I use a skybox.
	Gfx::clearBuffer(Gfx::ClearModeBit::ColorBuffer | Gfx::ClearModeBit::DepthBuffer);

	m_shader.setMat4("view", view);
	m_shader.setMat4("projection", projection);
	//m_shader.setFloat("time", Time::currentTime());
	m_shader.use();

	for (auto& [entity, component] : entityManager.getComponents<RenderingComponent>())
	{
		auto& position = entityManager.entityGetComponent<Position>(entity).value;
		auto& rotation = entityManager.entityGetComponent<Rotation>(entity).value;

		Mat4 model = rotation.rotationMatrix();
		model.setTranslation(position);
		m_shader.setMat4("model", model);

		Gfx::Primitives::cubeTrianglesVao->bind();
		Gfx::drawTriangles(0, 36);
	}
}
