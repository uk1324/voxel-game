#include <Engine/Renderer/Renderer.hpp>

static constexpr float untexturedCubeVertices[] = { -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f };
SkyboxData::SkyboxData(
	std::string_view right,
	std::string_view left,
	std::string_view top,
	std::string_view bottom,
	std::string_view front,
	std::string_view back)
	: cubeVbo(untexturedCubeVertices, sizeof(untexturedCubeVertices))
	, shader("src/Engine/Renderer/Skybox/skybox.vert", "src/Engine/Renderer/Skybox/skybox.frag")
	, texture(right, left, top, bottom, front, back)
{
	cubeVao.bind();
	cubeVbo.bind();
	Vao::setAttribute(0, ShaderDataType::Float, 3, false, sizeof(float) * 3, 0);
	shader.setTexture("skybox", 0);
}

void Renderer::drawSkybox(const Mat4& view, const Mat4& projection, SkyboxData& data)
{
	glActiveTexture(GL_TEXTURE0);
	data.texture.bind();

	data.shader.use();
	data.shader.setMat4("projection", projection);
	data.shader.setMat4("view", view.removedTranslation());

	data.cubeVao.bind();

	glDepthFunc(GL_LEQUAL);
	glFrontFace(GL_CCW);
	//glDepthMask(GL_FALSE);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glFrontFace(GL_CW);
	//glDepthMask(GL_TRUE);
}