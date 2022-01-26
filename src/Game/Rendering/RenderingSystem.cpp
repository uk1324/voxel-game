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
	, m_chunkShadowShader([]() {
		Gfx::ShaderProgram shader;
		shader.addShader(Gfx::Shader("src/Game/Rendering/Shaders/depthMap.vert", Gfx::ShaderType::Vertex));
		shader.addShader(Gfx::Shader("src/Game/Rendering/Shaders/depthMap.frag", Gfx::ShaderType::Fragment));
		shader.addShader(Gfx::Shader("src/Game/Rendering/Shaders/depthMap.geom", Gfx::ShaderType::Geometry));
		shader.link();
		return shader;
	}())
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

	shadowCascadeLevels = { farZ / 50.0f, farZ / 25.0f, farZ / 10.0f, farZ / 2.0f };

	glGenFramebuffers(1, &lightFBO);

	glGenTextures(1, &lightDepthMaps);
	glBindTexture(GL_TEXTURE_2D_ARRAY, lightDepthMaps);
	glTexImage3D(
		GL_TEXTURE_2D_ARRAY,
		0,
		GL_DEPTH_COMPONENT32F,
		depthMapResolution,
		depthMapResolution,
		int(shadowCascadeLevels.size()) + 1,
		0,
		GL_DEPTH_COMPONENT,
		GL_FLOAT,
		nullptr);

	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	constexpr float bordercolor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BORDER_COLOR, bordercolor);

	glBindFramebuffer(GL_FRAMEBUFFER, lightFBO);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, lightDepthMaps, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	int status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		//std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!";
		throw 0;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glGenBuffers(1, &matricesUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, matricesUBO);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(Mat4) * 16, nullptr, GL_STATIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, matricesUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

#include <Engine/Graphics/GraphicsPrimitives.hpp>
#include <iostream>
#include <imgui.h>

#include <string>

void RenderingSystem::update(const Vec2& screenSize, const Vec3& cameraPos, const Quat& cameraRot, const EntityManager& entityManger, const ChunkSystem& chunkSystem)
{
	static constexpr float fov = degToRad(90.0f);
	//static constexpr float nearZ = 0.1f;
	//static float farZ = sqrt(2.0f) * 4 * 16;
	const Mat4 projection = Mat4::perspective(fov, screenSize.x / screenSize.y, nearZ, farZ);
	const  Mat4 view = Mat4::lookAt(cameraPos, cameraPos + (cameraRot * Vec3::forward), Vec3::up);

	glViewport(0, 0, screenSize.x, screenSize.y);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	glActiveTexture(GL_TEXTURE0); 
	chunkSystem.blockData.textureArray.bind();
	m_chunkShader.setTexture("blockTextureArray", 0);

	auto lightSpaceMatrix = getLightSpaceMatrix(fov, nearZ, farZ, screenSize.x / screenSize.y, view);

	m_chunkShadowShader.use();
	//m_chunkShadowShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
	
	glViewport(0, 0, depthMapResolution, depthMapResolution);
	glBindFramebuffer(GL_FRAMEBUFFER, lightFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	glCullFace(GL_FRONT);
	drawChunks(chunkSystem, m_chunkShadowShader);
	glCullFace(GL_BACK);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	ImGui::Begin("abcd");
	ImGui::SliderFloat("1", &a1, 0, 80);
	ImGui::SliderFloat("2", &a2, 0, 80);
	ImGui::SliderFloat("3", &a3, 0, 80);
	ImGui::SliderFloat("4", &a4, 0, 80);
	//ImGui::SliderFloat("5", &a5, 0, 80);
	ImGui::End();
	//shadowCascadeLevels = { farZ / 50.0f, farZ / 25.0f, farZ / 10.0f, farZ / 2.0f };
	shadowCascadeLevels = { a1, a2, a3, a4 };

	const auto lightMatrices = getLightSpaceMatrices(fov, screenSize.x / screenSize.y, cameraPos, cameraRot);
	glBindBuffer(GL_UNIFORM_BUFFER, matricesUBO);
	for (size_t i = 0; i < lightMatrices.size(); ++i)
	{
		glBufferSubData(GL_UNIFORM_BUFFER, i * sizeof(Mat4), sizeof(Mat4), &lightMatrices[i]);
	}
	glBindBuffer(GL_UNIFORM_BUFFER, 0);


	// 2. then render scene as normal with shadow mapping (using depth map)
	glViewport(0, 0, screenSize.x, screenSize.y);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glActiveTexture(GL_TEXTURE1);
	//glBindTexture(GL_TEXTURE_2D, depthMap);

	m_chunkShader.use();

	glActiveTexture(GL_TEXTURE0);
	chunkSystem.blockData.textureArray.bind();
	m_chunkShader.setTexture("blockTextureArray", 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D_ARRAY, lightDepthMaps);
	m_chunkShader.setTexture("shadowMap", 1);

	m_chunkShader.setMat4("projection", projection);
	m_chunkShader.setMat4("camera", view);
	
	m_chunkShader.setFloat("farPlane", farZ);
	m_chunkShader.setInt("cascadeCount", shadowCascadeLevels.size());
	for (size_t i = 0; i < shadowCascadeLevels.size(); ++i)
	{
		m_chunkShader.setFloat("cascadePlaneDistances[" + std::to_string(i) + "]", shadowCascadeLevels[i]);
	}
	drawChunks(chunkSystem, m_chunkShader);

	glViewport(0, 0, 300, 300);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D_ARRAY, lightDepthMaps);
	m_squareTrianglesVao2.bind();
	m_texturedSquareShader.use();
	m_texturedSquareShader.setTexture("textureSampler", 0);
	m_texturedSquareShader.setVec2("pos", Vec2(0));
	m_texturedSquareShader.setVec2("size", Vec2(1));
	m_texturedSquareShader.setVec2("textureCoordScale", Vec2(1));
	ImGui::SliderInt("abc", &index, 0, 5);
	m_texturedSquareShader.setInt("index", index);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glViewport(0, 0, screenSize.x, screenSize.y);
	
	drawDebugShapes(projection, view);
	// Drawing the skybox last because it is behind everything so less fragments need to be drawn that way.
	drawSkybox(projection, view);
	drawCrosshair(screenSize);
}

void RenderingSystem::drawChunks(const ChunkSystem& chunkSystem, Gfx::ShaderProgram& shader)
{
	chunkSystem.m_vao.bind();
	for (const auto& chunk : chunkSystem.m_chunksToDraw)
	{
		shader.setMat4("model", Mat4::translation(Vec3(chunk->pos) * Chunk::SIZE));
		glDrawArrays(GL_TRIANGLES, chunk->vboByteOffset / sizeof(uint32_t), chunk->vertexCount);

		if (Debug::shouldShowChunkBorders)
		{
			Debug::drawCube(Vec3(chunk->pos) * Chunk::SIZE * Block::SIZE + Vec3(Chunk::SIZE) / 2.0, Vec3(Chunk::SIZE), Vec3(1, 1, 1));
		}
	}
}

void RenderingSystem::drawSkybox(const Mat4& projection, const Mat4& view)
{
	const Mat4 viewNoTranslation = view.removedTranslation();
	glActiveTexture(GL_TEXTURE0);
	m_skyboxTexture.bind();
	m_skyboxShader.setMat4("projection", projection);
	m_skyboxShader.setMat4("view", viewNoTranslation);
	m_skyboxShader.use();
	m_cubeTrianglesVao.bind();
	glDepthFunc(GL_LEQUAL);
	glFrontFace(GL_CCW);
	glDepthMask(GL_FALSE);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glFrontFace(GL_CW);
	glDepthMask(GL_TRUE);
}

void RenderingSystem::drawDebugShapes(const Mat4& projection, const Mat4& view)
{
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

void RenderingSystem::drawCrosshair(const Vec2& screenSize)
{
	// Crosshair
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	m_squareShader.use();
	m_squareTrianglesVao.bind();
	glActiveTexture(GL_TEXTURE0);
	m_crosshairTexture.bind();
	m_squareShader.setTexture("txt", 0);
	m_squareShader.setVec2("viewportSize", screenSize);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisable(GL_BLEND);
}

std::vector<Vec3> RenderingSystem::getFrustumCornersWorldSpace(const Mat4& proj, const Mat4& view)
{
	const auto inv = (view * proj).inverse();

	std::vector<Vec3> frustumCorners;
	// Generates ndc coordinates.
	// Maybe use std::array and put the points there instead of using a loop.
	for (unsigned int x = 0; x < 2; ++x)
	{
		for (unsigned int y = 0; y < 2; ++y)
		{
			for (unsigned int z = 0; z < 2; ++z)
			{
				// Already do perspective divide inside the multiplication function.
				frustumCorners.push_back(inv * Vec3(2.0f * x - 1.0f, 2.0f * y - 1.0f, 2.0f * z - 1.0f));
			}
		}
	}

	return frustumCorners;
}

Mat4 RenderingSystem::getLightSpaceMatrix(float fov, float nearZ, float farZ, float aspectRatio, const Mat4& view)
{
	Vec3 center(0, 0, 0);
	const auto projection = Mat4::perspective(fov, aspectRatio, nearZ, farZ);
	auto x = getFrustumCornersWorldSpace(projection, view);

	for (const auto& v : x)
	{
		//Debug::drawPoint(v);
		center += v;
	}
	center /= x.size();

	const auto lightView = Mat4::lookAt(
		center - Vec3(20.0f, 50, 20.0f).normalized(),
		center,
		Vec3(0.0f, 1.0f, 0.0f)
	);

	float minX = std::numeric_limits<float>::max();
	float maxX = std::numeric_limits<float>::min();
	float minY = std::numeric_limits<float>::max();
	float maxY = std::numeric_limits<float>::min();
	float minZ = std::numeric_limits<float>::max();
	float maxZ = std::numeric_limits<float>::min();
	for (const auto& v : x)
	{
		const auto trf = lightView * v;
		minX = std::min(minX, trf.x);
		maxX = std::max(maxX, trf.x);
		minY = std::min(minY, trf.y);
		maxY = std::max(maxY, trf.y);
		minZ = std::min(minZ, trf.z);
		maxZ = std::max(maxZ, trf.z);
	}


	constexpr float zMult = 1.0f;
	if (minZ < 0)
	{
		minZ *= zMult;
	}
	else
	{
		minZ /= zMult;
	}
	if (maxZ < 0)
	{
		maxZ /= zMult;
	}
	else
	{
		maxZ *= zMult;
	}

	Vec3 min(minX, minY, minZ); Vec3 max(maxX, maxY, maxZ);
	Debug::drawCube((min + max) / 2.0f, max - min);

	const Mat4 lightProjection = Mat4::orthographic(Vec3(minX, minY, minZ), Vec3(maxX, maxY, maxZ));

	const Mat4 lightSpaceMatrix = lightView * lightProjection;
	return lightSpaceMatrix;
}


std::vector<Mat4> RenderingSystem::getLightSpaceMatrices(float fov, float aspectRatio, const Vec3& pos, const Quat& rot)
{
	std::vector<Mat4> ret;
	for (size_t i = 0; i < shadowCascadeLevels.size() + 1; ++i)
	{
		auto view = Mat4::lookAt(pos, pos + rot * Vec3::forward, Vec3::up);
		if (i == 0)
		{
			/*Vec3 p = pos + (rot * Vec3::forward) * nearZ;
			auto view = Mat4::lookAt(p, p + rot * Vec3::forward, Vec3::up);*/
			ret.push_back(getLightSpaceMatrix(fov, nearZ, shadowCascadeLevels[i], aspectRatio, view));
		}
		else if (i < shadowCascadeLevels.size())
		{
			/*Vec3 p = pos + (rot * Vec3::forward) * shadowCascadeLevels[i - 1];
			auto view = Mat4::lookAt(p, p + rot * Vec3::forward, Vec3::up);*/
			ret.push_back(getLightSpaceMatrix(fov, shadowCascadeLevels[i - 1], shadowCascadeLevels[i], aspectRatio, view));
		}
		else
		{
			/*Vec3 p = pos + (rot * Vec3::forward) * shadowCascadeLevels[i - 1];
			auto view = Mat4::lookAt(p, p + rot * Vec3::forward, Vec3::up);*/
			ret.push_back(getLightSpaceMatrix(fov, shadowCascadeLevels[i - 1], farZ, aspectRatio, view));
		}
	}
	return ret;
}

