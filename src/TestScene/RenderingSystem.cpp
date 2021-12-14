#include <TestScene/RenderingSystem.hpp>
#include <Engine/Graphics/Drawing.hpp>
#include <Game/Components/Position.hpp>
#include <Game/Components/Rotation.hpp>

RenderingSystem::RenderingSystem()
	: m_shader("src/TestScene/shader.vert", "src/TestScene/shader.frag")
	, m_model("src/Model/Box.gltf")
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

#include <iostream>
#include <Engine/Time.hpp>

void RenderingSystem::update(const EntityManager& entityManager, const Mat4& view, const Mat4& projection)
{
	// Can remove clear ColorBuffer if I use a skybox.
	Gfx::clearBuffer(Gfx::ClearModeBit::ColorBuffer | Gfx::ClearModeBit::DepthBuffer);

	glDisable(GL_CULL_FACE);

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
		//std::cout << "vao: " << m_model.meshes[0].vao.handle() << '\n';
		m_model.meshes[0].vao.bind();
		m_model.m_buffers[0].bindAsIndexBuffer();

		glDrawElements(GL_POINTS, m_model.meshes[0].count, GL_UNSIGNED_SHORT, reinterpret_cast<void*>(m_model.meshes[0].offset));
		//glDrawArrays(GL_TRIANGLES, 0, 24);
	}
	//std::cout << m_model.meshes[0].count << ' ' << m_model.meshes[0].offset << '\n';
}
