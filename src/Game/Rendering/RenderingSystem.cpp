#include <Game/Rendering/RenderingSystem.hpp>
#include <Math/Angles.hpp>

// TODO: Maybe change this to range -0.5 to 0.5

static float cubeTrianglesVertices[] = { -1.0f,  1.0f, -1.0f,-1.0f, -1.0f, -1.0f,1.0f, -1.0f, -1.0f,1.0f, -1.0f, -1.0f,1.0f,  1.0f, -1.0f,-1.0f,  1.0f, -1.0f,-1.0f, -1.0f,  1.0f,-1.0f, -1.0f, -1.0f,-1.0f,  1.0f, -1.0f,-1.0f,  1.0f, -1.0f,-1.0f,  1.0f,  1.0f,-1.0f, -1.0f,  1.0f,1.0f, -1.0f, -1.0f,1.0f, -1.0f,  1.0f,1.0f,  1.0f,  1.0f,1.0f,  1.0f,  1.0f,1.0f,  1.0f, -1.0f,1.0f, -1.0f, -1.0f,-1.0f, -1.0f,  1.0f,-1.0f,  1.0f,  1.0f,1.0f,  1.0f,  1.0f,1.0f,  1.0f,  1.0f,1.0f, -1.0f,  1.0f,-1.0f, -1.0f,  1.0f,-1.0f,  1.0f, -1.0f,1.0f,  1.0f, -1.0f,1.0f,  1.0f,  1.0f,1.0f,  1.0f,  1.0f,-1.0f,  1.0f,  1.0f,-1.0f,  1.0f, -1.0f,-1.0f, -1.0f, -1.0f,-1.0f, -1.0f,  1.0f,1.0f, -1.0f, -1.0f,1.0f, -1.0f, -1.0f,-1.0f, -1.0f,  1.0f,1.0f, -1.0f,  1.0f };
static float cubeLinesVertices[] = { 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5, -0.5, -0.5, -0.5, -0.5, -0.5, 0.5, -0.5, -0.5, 0.5,-0.5,0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, -0.5, -0.5, -0.5, -0.5, -0.5, 0.5, -0.5, -0.5,0.5,0.5, -0.5,-0.5,0.5, -0.5, -0.5, -0.5, -0.5,-0.5,0.5, 0.5,-0.5,0.5, -0.5,-0.5,0.5, -0.5,0.5,0.5, 0.5,0.5,0.5, 0.5, 0.5, -0.5, 0.5,0.5,0.5, 0.5,-0.5,0.5, 0.5,0.5,0.5, -0.5,0.5,0.5, };
static float squareTrianglesVertices[] = { -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f };
static float point[] = { 0.0f, 0.0f, 0.0f };

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
	, m_blockTextureArray(
		16, 16,
		{
			"assets/textures/blocks/dirt.png",
			"assets/textures/blocks/stone.png",
			"assets/textures/blocks/cobblestone.png",
			"assets/textures/blocks/grass.png",
			"assets/textures/blocks/grass2.png"
		}
	)
	, m_chunkShader("src/Game/Rendering/Shaders/chunk.vert", "src/Game/Rendering/Shaders/chunk.frag")
	, m_crosshairTexture("assets/textures/crosshair.png")
	, m_squareShader("src/Game/Rendering/Shaders/2dTextured.vert", "src/Game/Rendering/Shaders/2dTextured.frag")
	, m_debugShader("src/Game/Rendering/Shaders/debug.vert", "src/Game/Rendering/Shaders/debug.frag")
	, m_cubeTrianglesVbo(cubeTrianglesVertices, sizeof(cubeTrianglesVertices))
	, m_cubeLinesVbo(cubeLinesVertices, sizeof(cubeLinesVertices))
	, m_squareTrianglesVbo(squareTrianglesVertices, sizeof(squareTrianglesVertices))
	, m_pointVbo(point, sizeof(point))
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

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_PROGRAM_POINT_SIZE);
	//glEnable(GL_CULL_FACE);
	Debug::renderingSystem = this;
}

#include <Engine/Graphics/GraphicsPrimitives.hpp>
#include <iostream>

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
	m_blockTextureArray.bind();
	m_chunkShader.setTexture("blockTextureArray", 0);
	m_chunkShader.setMat4("camera", view);
	m_chunkShader.setMat4("projection", projection);
	m_chunkShader.use();
	for (const auto& chunk : chunkSystem.m_chunksToDraw)
	{
		m_chunkShader.setMat4("model", Mat4::translation(Vec3(chunk->pos) * Chunk::SIZE));
		chunkSystem.m_vao.bind();
		glDrawArrays(GL_TRIANGLES, chunk->vboByteOffset / sizeof(uint32_t), chunk->vertexCount);
		// Chunk borders
		//Debug::drawCube(chunk->pos * Chunk::SIZE * Block::SIZE + Vec3(Chunk::SIZE) / 2, Vec3(Chunk::SIZE));
	} 


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
	glEnable(GL_DEPTH_TEST);
	glFrontFace(GL_CW);
	glDepthMask(GL_TRUE);

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
	
	glLineWidth(2);
	m_cubeLinesVao.bind();
	m_debugShader.use();
	m_debugShader.setMat4("view", view);
	m_debugShader.setMat4("projection", projection);
	for (const Cube& cube : m_cubesToDraw)
	{
		Mat4 scale = Mat4::identity;
		scale.scale(cube.scale);
		m_debugShader.setMat4("model", scale * Mat4::translation(cube.pos));
		m_debugShader.setVec3("color", cube.color);
		glDrawArrays(GL_LINES, 0, (sizeof(cubeLinesVertices) / (sizeof(float) * 3)));
	}
	m_cubesToDraw.clear();

	for (const Point& point : m_pointsToDraw)
	{
		glPointSize(point.size);
		m_debugShader.setMat4("model", Mat4::translation(point.pos));
		m_debugShader.setVec3("color", point.color);
		glDrawArrays(GL_POINTS, 0, 1);
	}
	m_pointsToDraw.clear();
}

void RenderingSystem::drawCube(const Vec3& pos, const Vec3& scale, const Vec3& color)
{
	m_cubesToDraw.push_back(Cube{ pos, scale, color });
}

void RenderingSystem::drawPoint(const Vec3& pos, float size, const Vec3& color)
{
	m_pointsToDraw.push_back(Point{ pos, size, color });
}

void Debug::drawCube(const Vec3& pos, const Vec3& scale, const Vec3& color)
{
	renderingSystem->drawCube(pos, scale, color);
}

void Debug::drawPoint(const Vec3& pos, float size, const Vec3& color)
{
	renderingSystem->drawPoint(pos, size, color);
}

RenderingSystem* Debug::renderingSystem = nullptr;