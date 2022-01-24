#include <Game/Rendering/RenderingSystem.hpp>
#include <Math/Angles.hpp>
#include <Game/Debug/Debug.hpp>

// TODO: Maybe change this to range -0.5 to 0.5

static float cubeTrianglesVertices[] = { -1.0f,  1.0f, -1.0f,-1.0f, -1.0f, -1.0f,1.0f, -1.0f, -1.0f,1.0f, -1.0f, -1.0f,1.0f,  1.0f, -1.0f,-1.0f,  1.0f, -1.0f,-1.0f, -1.0f,  1.0f,-1.0f, -1.0f, -1.0f,-1.0f,  1.0f, -1.0f,-1.0f,  1.0f, -1.0f,-1.0f,  1.0f,  1.0f,-1.0f, -1.0f,  1.0f,1.0f, -1.0f, -1.0f,1.0f, -1.0f,  1.0f,1.0f,  1.0f,  1.0f,1.0f,  1.0f,  1.0f,1.0f,  1.0f, -1.0f,1.0f, -1.0f, -1.0f,-1.0f, -1.0f,  1.0f,-1.0f,  1.0f,  1.0f,1.0f,  1.0f,  1.0f,1.0f,  1.0f,  1.0f,1.0f, -1.0f,  1.0f,-1.0f, -1.0f,  1.0f,-1.0f,  1.0f, -1.0f,1.0f,  1.0f, -1.0f,1.0f,  1.0f,  1.0f,1.0f,  1.0f,  1.0f,-1.0f,  1.0f,  1.0f,-1.0f,  1.0f, -1.0f,-1.0f, -1.0f, -1.0f,-1.0f, -1.0f,  1.0f,1.0f, -1.0f, -1.0f,1.0f, -1.0f, -1.0f,-1.0f, -1.0f,  1.0f,1.0f, -1.0f,  1.0f };
static float cubeLinesVertices[] = { 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5, -0.5, -0.5, -0.5, -0.5, -0.5, 0.5, -0.5, -0.5, 0.5,-0.5,0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, -0.5, -0.5, -0.5, -0.5, -0.5, 0.5, -0.5, -0.5,0.5,0.5, -0.5,-0.5,0.5, -0.5, -0.5, -0.5, -0.5,-0.5,0.5, 0.5,-0.5,0.5, -0.5,-0.5,0.5, -0.5,0.5,0.5, 0.5,0.5,0.5, 0.5, 0.5, -0.5, 0.5,0.5,0.5, 0.5,-0.5,0.5, 0.5,0.5,0.5, -0.5,0.5,0.5, };
static float squareTrianglesVertices[] = { -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f };
static float point[] = { 0.0f, 0.0f, 0.0f };
static float lineData[] = { 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f };

static const float squareTrianglesVertices2[] = {
	// vertexPos  textureCoord
	-1.0f,  1.0F, 0.0f, 1.0f,
	 1.0f, -1.0f, 1.0f, 0.0f,
	-1.0f, -1.0f, 0.0f, 0.0f,
	-1.0f,  1.0f, 0.0f, 1.0f,
	 1.0f,  1.0f, 1.0f, 1.0f,
	 1.0f, -1.0f, 1.0f, 0.0f,
};

RenderingSystem::RenderingSystem(Scene& scene)
	: m_skyboxTexture(Gfx::CubeMapTexturePaths{
		"assets/textures/skybox/right.png",
		"assets/textures/skybox/left.png",
		"assets/textures/skybox/top.png",
		"assets/textures/skybox/bottom.png",
		"assets/textures/skybox/front.png",
		"assets/textures/skybox/back.png"
		})
	, m_skyboxShader("src/Game/Rendering/Shaders/skybox.vert", "src/Game/Rendering/Shaders/skybox.frag")
	, m_chunkShader("src/Game/Rendering/Shaders/chunk.vert", "src/Game/Rendering/Shaders/chunk.frag")
	, m_chunkShadowShader("src/Game/Rendering/Shaders/depthMap.vert", "src/Game/Rendering/Shaders/depthMap.frag")
	, m_crosshairTexture("assets/textures/crosshair.png")
	, m_squareShader("src/Game/Rendering/Shaders/2dTextured.vert", "src/Game/Rendering/Shaders/2dTextured.frag")
	, m_debugShader("src/Game/Rendering/Shaders/debug.vert", "src/Game/Rendering/Shaders/debug.frag")
	, m_texturedSquareShader("src/Game/Inventory/Shaders/uiTextured.vert", "src/Game/Rendering/Shaders/debugDepth.frag")
	, m_cubeTrianglesVbo(cubeTrianglesVertices, sizeof(cubeTrianglesVertices))
	, m_cubeLinesVbo(cubeLinesVertices, sizeof(cubeLinesVertices))
	, m_squareTrianglesVbo(squareTrianglesVertices, sizeof(squareTrianglesVertices))
	, m_pointVbo(point, sizeof(point))
	, m_LineVbo(lineData, sizeof(lineData))
	, m_squareTrianglesVbo2(squareTrianglesVertices2, sizeof(squareTrianglesVertices2))
{
	m_cubeTrianglesVao.bind();
	m_cubeTrianglesVbo.bind();
	Gfx::VertexArray::setAttribute(0, Gfx::BufferLayout(Gfx::ShaderDataType::Float, 3, 0, sizeof(float) * 3, false));
	m_cubeLinesVao.bind();
	m_cubeLinesVbo.bind();
	Gfx::VertexArray::setAttribute(0, Gfx::BufferLayout(Gfx::ShaderDataType::Float, 3, 0, sizeof(float) * 3, false));
	m_squareTrianglesVao.bind();
	m_squareTrianglesVbo.bind();
	Gfx::VertexArray::setAttribute(0, Gfx::BufferLayout(Gfx::ShaderDataType::Float, 2, 0, sizeof(float) * 2, false));
	m_pointVao.bind();
	m_pointVbo.bind();
	Gfx::VertexArray::setAttribute(0, Gfx::BufferLayout(Gfx::ShaderDataType::Float, 3, 0, sizeof(float) * 3, false));
	m_LineVao.bind();
	m_LineVbo.bind();
	Gfx::VertexArray::setAttribute(0, Gfx::BufferLayout(Gfx::ShaderDataType::Float, 3, 0, sizeof(float) * 3, false));

	m_squareTrianglesVao2.bind();
	m_squareTrianglesVbo2.bind();
	Gfx::VertexArray::setAttribute(0, Gfx::BufferLayout(Gfx::ShaderDataType::Float, 2, 0, sizeof(float) * 4, false));
	Gfx::VertexArray::setAttribute(1, Gfx::BufferLayout(Gfx::ShaderDataType::Float, 2, sizeof(float) * 2, sizeof(float) * 4, false));

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_PROGRAM_POINT_SIZE);
	glEnable(GL_CULL_FACE);

	glGenFramebuffers(1, &depthMapFBO);
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

#include <Engine/Graphics/GraphicsPrimitives.hpp>
#include <iostream>
#include <imgui.h>

void RenderingSystem::update(float width, float height, const Vec3& cameraPos, const Quat& cameraRot, const EntityManager& entityManger, const ChunkSystem& chunkSystem)
{
	static constexpr float fov = degToRad(90.0f);
	static constexpr float nearZ = 0.1f;
	static constexpr float farZ = 1000.0f;
	const Mat4 projection = Mat4::perspective(fov, width / height, nearZ, farZ);
	const Mat4 view = Mat4::lookAt(cameraPos, cameraPos + (cameraRot * Vec3::forward), Vec3::up);

	glViewport(0, 0, width, height);
	// Not clearing color buffer because a skybox is drawn.
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	// Chunks
	glActiveTexture(GL_TEXTURE0);
	//m_blockTextureArray.bind();
	chunkSystem.blockData.textureArray.bind();
	m_chunkShader.setTexture("blockTextureArray", 0);

	ImGui::Begin("asdf");
	ImGui::SliderFloat("x", &p.x, -10, 10);
	ImGui::SliderFloat("y", &p.y, -10, 10);
	ImGui::SliderFloat("z", &p.z, -10, 10);
	ImGui::End();

	float near_plane = 1.0f, far_plane = 7.5f;
	auto lightProjection = Mat4::orthographic(Vec3(-10.0f, -10.0f, near_plane), Vec3(10.0f, 10.0f, far_plane));
	
	auto lightView = Mat4::lookAt(p, Vec3(0.0f, 1.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f));
	//auto lightView = view;

	Debug::drawCube(p);
	Debug::drawLine(p, Vec3(0.0f, 1.0f, 0.0f));

	Mat4 lightSpaceMatrix = lightProjection * lightView;

	m_chunkShadowShader.use();
	m_chunkShadowShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
	//glActiveTexture(GL_TEXTURE0);
	//chunkSystem.blockData.textureArray.bind();
	//m_chunkShadowShader.setTexture("blockTextureArray", 0);
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	glCullFace(GL_FRONT);
	{
		for (const auto& chunk : chunkSystem.m_chunksToDraw)
		{
			m_chunkShadowShader.setMat4("model", Mat4::translation(Vec3(chunk->pos) * Chunk::SIZE));
			chunkSystem.m_vao.bind();
			glDrawArrays(GL_TRIANGLES, chunk->vboByteOffset / sizeof(uint32_t), chunk->vertexCount);

			if (Debug::shouldShowChunkBorders)
			{
				Debug::drawCube(Vec3(chunk->pos) * Chunk::SIZE * Block::SIZE + Vec3(Chunk::SIZE) / 2.0, Vec3(Chunk::SIZE), Vec3(1, 1, 1));
			}
		}
	}
	glCullFace(GL_BACK);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	//// 2. then render scene as normal with shadow mapping (using depth map)
	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	m_chunkShader.setTexture("shadowMap", 1);
	m_chunkShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
	{
		m_chunkShader.setMat4("camera", view);
		m_chunkShader.setMat4("projection", projection);
		m_chunkShader.use();
		for (const auto& chunk : chunkSystem.m_chunksToDraw)
		{
			m_chunkShader.setMat4("model", Mat4::translation(Vec3(chunk->pos) * Chunk::SIZE));
			chunkSystem.m_vao.bind();
			glDrawArrays(GL_TRIANGLES, chunk->vboByteOffset / sizeof(uint32_t), chunk->vertexCount);

			if (Debug::shouldShowChunkBorders)
			{
				Debug::drawCube(Vec3(chunk->pos) * Chunk::SIZE * Block::SIZE + Vec3(Chunk::SIZE) / 2.0, Vec3(Chunk::SIZE), Vec3(1, 1, 1));
			}
		}
	}

	glViewport(0, 0, 300, 300);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	m_squareTrianglesVao2.bind();
	m_texturedSquareShader.use();
	m_texturedSquareShader.setTexture("textureSampler", 0);
	m_texturedSquareShader.setVec2("pos", Vec2(0));
	m_texturedSquareShader.setVec2("size", Vec2(1));
	m_texturedSquareShader.setVec2("textureCoordScale", Vec2(1));
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glViewport(0, 0, width, height);

	// Skybox
	// Drawing after everything because is behind everything so less fragments need to be drawn.
	const Mat4 viewNoTranslation = view.removedTranslation();
	glActiveTexture(GL_TEXTURE0);
	m_skyboxTexture.bind();
	//m_skyboxShader.setTexture("skybox", 0); 
	m_skyboxShader.setMat4("projection", projection);
	m_skyboxShader.setMat4("view", viewNoTranslation);
	m_skyboxShader.use();
	//Gfx::Primitives::cubeTrianglesVao->bind();
	m_cubeTrianglesVao.bind();
	glDepthFunc(GL_LEQUAL);
	glFrontFace(GL_CCW);
	glDepthMask(GL_FALSE);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	//glEnable(GL_DEPTH_TEST);
	glFrontFace(GL_CW);
	glDepthMask(GL_TRUE);

	glLineWidth(2);
	m_cubeLinesVao.bind();
	m_debugShader.use();
	m_debugShader.setMat4("view", view);
	m_debugShader.setMat4("projection", projection);

	for (const Cube& cube : m_cubesToDraw)
	{
		m_debugShader.setMat4("model", Mat4::scale(cube.scale) * Mat4::translation(cube.pos));
		m_debugShader.setVec3("color", cube.color);
		glDrawArrays(GL_LINES, 0, (sizeof(cubeLinesVertices) / (sizeof(float) * 3)));
	}
	m_cubesToDraw.clear();

	m_pointVao.bind();
	for (const Point& point : m_pointsToDraw)
	{
		glPointSize(point.size);
		m_debugShader.setMat4("model", Mat4::translation(point.pos));
		m_debugShader.setVec3("color", point.color);
		glDrawArrays(GL_POINTS, 0, 1);
	}
	m_pointsToDraw.clear();

	m_LineVao.bind();
	for (const Line& line : m_linesToDraw)
	{
		m_debugShader.setMat4("model", Mat4::scale(line.scale) * Mat4::translation(line.startPos));
		m_debugShader.setVec3("color", line.color);
		glDrawArrays(GL_LINES, 0, (sizeof(lineData) / (sizeof(float) * 3)));
	}
	m_linesToDraw.clear();


	// Crosshair
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	m_squareShader.use();
	m_squareTrianglesVao.bind();
	glActiveTexture(GL_TEXTURE0);
	m_crosshairTexture.bind();
	m_squareShader.setTexture("txt", 0);
	m_squareShader.setVec2("viewportSize", Vec2(width, height));
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisable(GL_BLEND);
}

void RenderingSystem::drawCube(const Vec3& pos, const Vec3& scale, const Vec3& color)
{
	m_cubesToDraw.push_back(Cube{ pos, scale, color });
}

void RenderingSystem::drawPoint(const Vec3& pos, float size, const Vec3& color)
{
	m_pointsToDraw.push_back(Point{ pos, size, color });
}

void RenderingSystem::drawLine(const Vec3& startPos, const Vec3& endPos, const Vec3& color)
{
	m_linesToDraw.push_back(Line{ startPos, endPos - startPos, color });
}
