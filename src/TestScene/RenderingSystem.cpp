#include <TestScene/RenderingSystem.hpp>
#include <Engine/Graphics/Drawing.hpp>
#include <Game/Components/Position.hpp>
#include <Game/Components/Rotation.hpp>

RenderSystem::RenderSystem()
	: m_shader("src/TestScene/shader.vert", "src/TestScene/shader.frag")
	, m_model("src/Model/Duck.gltf")
{
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glEnable(GL_CULL_FACE);

}

#include <iostream>
#include <Engine/Time.hpp>

void RenderSystem::update(const EntityManager& entityManager, Entity player, const Mat4& view, const Mat4& projection)
{
	// Can remove clear ColorBuffer if I use a skybox.
	//Gfx::clearBuffer(Gfx::ClearModeBit::ColorBuffer | Gfx::ClearModeBit::DepthBuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_shader.setMat4("view", view);
	m_shader.setMat4("projection", projection);
	m_shader.setVec3("lookDir", entityManager.entityGetComponent<Rotation>(player).value * Vec3(0, 0, 1));
	//m_shader.setFloat("time", Time::currentTime());
	
	m_shader.use();

	for (auto& [entity, component] : entityManager.getComponents<RenderingComponent>())
	{
		const auto& position = entityManager.entityGetComponent<Position>(entity).value;
		const auto& rotation = entityManager.entityGetComponent<Rotation>(entity).value;

		Mat4 model = rotation.rotationMatrix();
		model.scale(Vec3(0.02, 0.02, 0.02));
		model.setTranslation(position);
		m_shader.setMat4("model", model);

		//Gfx::Primitives::cubeTrianglesVao->bind();
		//Gfx::drawTriangles(0, 36);
		//std::cout << "vao: " << m_model.meshes[0].vao.handle() << '\n';
		m_model.m_textures[0].bind();
		m_shader.setInt("tex", 0);
		m_model.meshes[0].vao.bind();
		m_model.m_buffers[0].bindAsIndexBuffer();

		//glDrawElements(GL_POINTS, m_model.meshes[0].count, GL_UNSIGNED_SHORT, reinterpret_cast<void*>(m_model.meshes[0].offset));
		glDrawElements(GL_TRIANGLES, m_model.meshes[0].count, GL_UNSIGNED_SHORT, reinterpret_cast<void*>(m_model.meshes[0].offset));
		//glPointSize(10);
		//glDrawArrays(GL_POINTS, 0, 24);
	}
	//std::cout << m_model.meshes[0].count << ' ' << m_model.meshes[0].offset << '\n';
}
