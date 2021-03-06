#include <Game/Rendering/RenderingSystem.hpp>
#include <Game/Components/Position.hpp>
#include <Game/Debug/Debug.hpp>
#include <Model/ModelLoader.hpp>
#include <Game/EntitySystem.hpp>

#include <string>

// TODO: Maybe change this to range -0.5 to 0.5

static float cubeLinesVertices[] = { 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5, -0.5, -0.5, -0.5, -0.5, -0.5, 0.5, -0.5, -0.5, 0.5,-0.5,0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, -0.5, -0.5, -0.5, -0.5, -0.5, 0.5, -0.5, -0.5,0.5,0.5, -0.5,-0.5,0.5, -0.5, -0.5, -0.5, -0.5,-0.5,0.5, 0.5,-0.5,0.5, -0.5,-0.5,0.5, -0.5,0.5,0.5, 0.5,0.5,0.5, 0.5, 0.5, -0.5, 0.5,0.5,0.5, 0.5,-0.5,0.5, 0.5,0.5,0.5, -0.5,0.5,0.5, };
static float squareTrianglesVertices[] = { -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f };
static float point[] = { 0.0f, 0.0f, 0.0f };
static float lineData[] = { 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f };

static const float cubeTriangleVertices[] = {
									  // Face index
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0,
	 0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0,
	-0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0,
	 0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0,

	 0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1,
	-0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1,
	 0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1,
	 0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1,
	-0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1,

	-0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 2,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 2,
	-0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 2,
	-0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 2,
	-0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 2,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 2,

	 0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 3,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 3,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 3,
	 0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 3,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 3,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 3,

	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 4,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 4,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 4,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 4,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 4,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 4,

	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 5,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 5,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 5,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 5,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 5,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 5,
};

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
	: m_skyboxData(
		"assets/textures/skybox/right.png",
		"assets/textures/skybox/left.png",
		"assets/textures/skybox/top.png",
		"assets/textures/skybox/bottom.png",
		"assets/textures/skybox/front.png",
		"assets/textures/skybox/back.png")
	, m_chunkShader("src/Game/Rendering/Shaders/chunk.vert", "src/Game/Rendering/Shaders/chunk.frag")
	, m_chunkShadowShader(
		"src/Game/Rendering/Shaders/depthMap.vert",
		"src/Game/Rendering/Shaders/depthMap.geom",
		"src/Game/Rendering/Shaders/depthMap.frag")
	, m_crosshairTexture("assets/textures/crosshair.png")
	, m_squareShader("src/Game/Rendering/Shaders/2dTextured.vert", "src/Game/Rendering/Shaders/2dTextured.frag")
	, m_debugShader("src/Game/Rendering/Shaders/debug.vert", "src/Game/Rendering/Shaders/debug.frag")
	, m_texturedSquareShader("src/Game/Inventory/Shaders/uiTextured.vert", "src/Game/Rendering/Shaders/debugDepth.frag")
	, m_fboShader("src/Game/Rendering/Shaders/fbo.vert", "src/Game/Rendering/Shaders/fbo.frag")
	, m_cubeLinesVbo(cubeLinesVertices, sizeof(cubeLinesVertices))
	, m_squareTrianglesVbo(squareTrianglesVertices, sizeof(squareTrianglesVertices))
	, m_pointVbo(point, sizeof(point))
	, m_LineVbo(lineData, sizeof(lineData))
	, m_squareTrianglesVbo2(squareTrianglesVertices2, sizeof(squareTrianglesVertices2))

	, m_shadowMapTextures(TextureArray::incomplete())
	, m_shadowMapFbo(Fbo::incomplete())

	, m_fbo(Fbo::incomplete())
	, m_fboTexture(Texture::incomplete())
	, m_fboDepthTexture(Texture::incomplete())

	, m_modelShader("src/Game/Rendering/Shaders/model.vert", "src/Game/Rendering/Shaders/model.frag")
	//, m_model("assets/models/character/character.gltf")
	//, m_model(ModelLoader("src/model/duck.gltf").parse())
	, m_model(ModelLoader("assets/models/character/character.gltf").parse())
	, m_itemModelShader("src/Game/Rendering/Shaders/itemModel.vert", "src/Game/Rendering/Shaders/itemModel.frag")
	, m_itemBlockShader("src/Game/Rendering/Shaders/itemBlock.vert", "src/Game/Rendering/Shaders/itemBlock.frag")
	, m_cubeTrianglesVbo(cubeTriangleVertices, sizeof(cubeTriangleVertices))
{
	m_cubeLinesVao.bind();
	m_cubeLinesVbo.bind();
	Vao::setAttribute(0, BufferLayout(ShaderDataType::Float, 3, 0, sizeof(float) * 3, false));
	m_squareTrianglesVao.bind();
	m_squareTrianglesVbo.bind();
	Vao::setAttribute(0, BufferLayout(ShaderDataType::Float, 2, 0, sizeof(float) * 2, false));
	m_pointVao.bind();
	m_pointVbo.bind();
	Vao::setAttribute(0, BufferLayout(ShaderDataType::Float, 3, 0, sizeof(float) * 3, false));
	m_LineVao.bind();
	m_LineVbo.bind();
	Vao::setAttribute(0, BufferLayout(ShaderDataType::Float, 3, 0, sizeof(float) * 3, false));

	m_squareTrianglesVao2.bind();
	m_squareTrianglesVbo2.bind();
	Vao::setAttribute(0, BufferLayout(ShaderDataType::Float, 2, 0, sizeof(float) * 4, false));
	Vao::setAttribute(1, BufferLayout(ShaderDataType::Float, 2, sizeof(float) * 2, sizeof(float) * 4, false));

	m_cubeTrianglesVao.bind();
	m_cubeTrianglesVbo.bind();
	Vao::setAttribute(0, BufferLayout(ShaderDataType::Float, 3, 0, sizeof(float) * 6, false));
	Vao::setAttribute(1, BufferLayout(ShaderDataType::Float, 2, sizeof(float) * 3, sizeof(float) * 6, false));
	Vao::setAttribute(2, BufferLayout(ShaderDataType::Float, 1, sizeof(float) * 5, sizeof(float) * 6, false));

	onScreenResize();

	m_fbo.bind();

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_fboTexture.handle(), 0);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_fboDepthTexture.handle(), 0);

	m_fbo.unbind();

	shadowMapSetup();

	glEnable(GL_CULL_FACE);
}

// Draw skeleton with propagate transform in the model loader.
static void draw(const Vec3& pos, Model::Node& node, const Vec3& old)
{
	Vec3 transformed = node.output * Vec3(0) + old;
	Debug::drawLine(pos, transformed);
	for (const auto& child : node.children)
	{
		draw(transformed, *child, old);
		Debug::drawPoint(transformed);
	}
}

static void drawSkeleton(const Vec3& translation, Model::Node& node, const Mat4& parentTransform, const Vec3& parentTransformed)
{
	const Mat4 thisTransform = node.output * parentTransform;
	const Vec3 transformed = thisTransform * Vec3(0) + translation;
	Debug::drawLine(parentTransformed, transformed);
	for (const auto& child : node.children)
	{
		drawSkeleton(translation, *child, thisTransform, transformed);
		Debug::drawPoint(transformed);
	}
}

#include <iostream>
#include <imgui.h>

void RenderingSystem::update(const Vec2& screenSize, const Vec3& cameraPos, const Quat& cameraRot, const EntityManager& entityManger, const ChunkSystem& chunkSystem, const ItemData& itemData)
{
	if (m_screenSize != screenSize)
	{
		m_screenSize = screenSize;
		onScreenResize();
	}
	const Vec3 cameraDir = cameraRot * Vec3::forward;
	float aspectRatio = screenSize.x / screenSize.y;
	static constexpr float fov = degToRad(90.0f);
	const Mat4 projection = Mat4::perspective(fov, aspectRatio, m_nearPlaneZ, m_farPlaneZ);
	const  Mat4 view = Mat4::lookAt(cameraPos, cameraPos + cameraDir, Vec3::up);
	m_projection = projection;
	m_view = view;

	m_frustum = Frustum::fromCamera(cameraPos, cameraDir, aspectRatio, fov, m_nearPlaneZ, m_farPlaneZ);

	glEnable(GL_DEPTH_TEST);
	glClear(GL_DEPTH_BUFFER_BIT);

	const auto lightMatrices = getLightSpaceMatrices(fov, aspectRatio, view, m_directionalLightDir, m_nearPlaneZ, m_farPlaneZ, shadowCascadeLevels);
	glBindBuffer(GL_UNIFORM_BUFFER, matricesUBO);
	for (size_t i = 0; i < lightMatrices.size(); i++)
	{
		glBufferSubData(GL_UNIFORM_BUFFER, i * sizeof(Mat4), sizeof(Mat4), &lightMatrices[i]);
	}
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	drawToShadowMap(chunkSystem, entityManger, itemData); 

	m_fbo.bind();

	drawScene(chunkSystem, entityManger, itemData);
	//for (m_model.
	//for (auto& node : m_model.nodes)
	//{
	//	node.output = Mat4::identity;
	//	
	//}

	keyframe += 1;
	keyframe %= m_model.keyframes.size() * 2;

	for (size_t i = 0; i < m_model.nodes.size(); i++)
	{
		m_model.nodes[i].output = Mat4::scale(m_model.keyframes[keyframe / 2].scale[i])
			* m_model.keyframes[keyframe / 2].rotation[i].asMatrix()
			* Mat4::translation(m_model.keyframes[keyframe / 2].translation[i]);
	}

	ModelLoader::propagateTransform(m_model.nodes[18]);

	for (auto [entity, model] : entityManger.getComponents<ModelComponent>())
	{
		const Vec3& pos = entityManger.getComponent<Position>(entity).value;

		m_modelShader.use();
		m_modelShader.setMat4("model", Mat4::scale(Vec3(0.2)) * Mat4::translation(pos));
		m_modelShader.setMat4("view", view);
		m_modelShader.setMat4("projection", projection);
		glActiveTexture(GL_TEXTURE0);
		m_model.textures[0].bind();
		m_modelShader.setInt("textureSampler", 0);
		m_model.meshes[0].vao.bind();
		m_model.buffers[0].bindAsIndexBuffer();

		for (size_t i = 0; i < m_model.joints.size(); i++)
		{
			m_modelShader.setMat4("jointMatrices[" + std::to_string(i) + "]",
				m_model.inverseBindMatrices[i] * m_model.joints[i]->output);

			// First the inverseBindMatrix is applied which undoes the bind pose translating every joint (not vertex) to the zero vector
			// in object space. Then the transform is applied putting the mesh in the skeleton pose.
		}
		glFrontFace(GL_CCW);
		glDrawElements(GL_TRIANGLES, m_model.meshes[0].indicesCount, static_cast<GLenum>(m_model.meshes[0].indexType), reinterpret_cast<void*>(m_model.meshes[0].indicesByteOffset));
		glFrontFace(GL_CW);
	}

	drawDebugShapes(projection, view);

	Renderer::drawSkybox(view, projection, m_skyboxData);

	m_fbo.unbind();

	m_squareTrianglesVao2.bind();
	m_fboShader.use();
	glActiveTexture(GL_TEXTURE0);
	m_fboTexture.bind();
	m_fboShader.setTexture("textureSampler", 0);
	m_fboShader.setFloat("time", Time::currentTime());
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glClear(GL_DEPTH_BUFFER_BIT);
	m_itemModelShader.setMat4("projection", m_projection);
	m_itemModelShader.setMat4("view", m_view);
	//m_itemModelShader.setMat4("model", Mat4::translation(cameraPos));
	//const auto& itemInfo = itemData[itemComponent.item.item];
	const auto& itemInfo = itemData[ItemType::DiamondSword];
	//if (itemInfo.isBlock)
	//{
	//	//m_itemBlockShader.setMat4("model",
	//	//	Quat(timeSinceSpawned, Vec3::yAxis).asMatrix()
	//	//	* Mat4::scale(Vec3(0.25f))
	//	//	* Mat4::translation(pos + Vec3(0.0f, (sin(timeSinceSpawned) + 1.0f) / 10.0f, 0.0)));

	//	//const auto& blockInfo = chunkSystem.blockData[itemInfo.blockType];
	//	//m_itemBlockShader.use();
	//	//m_itemBlockShader.setUnsignedInt("faceTextureIndex[0]", blockInfo.frontTextureIndex);
	//	//m_itemBlockShader.setUnsignedInt("faceTextureIndex[1]", blockInfo.backTextureIndex);
	//	//m_itemBlockShader.setUnsignedInt("faceTextureIndex[2]", blockInfo.rightTextureIndex);
	//	//m_itemBlockShader.setUnsignedInt("faceTextureIndex[3]", blockInfo.leftTextureIndex);
	//	//m_itemBlockShader.setUnsignedInt("faceTextureIndex[4]", blockInfo.bottomTextureIndex);
	//	//m_itemBlockShader.setUnsignedInt("faceTextureIndex[5]", blockInfo.topTextureIndex);
	//	//m_cubeTrianglesVao.bind();
	//	//glDrawArrays(GL_TRIANGLES, 0, 36);
	//}
	//else

	ImGui::Begin("adfs");

	ImGui::SliderFloat3("abc", v.data(), -2, 2);
	ImGui::SliderFloat("a", &angl, -180, 180);

	ImGui::End();

	{
		m_itemModelShader.setMat4("model", 
			(cameraRot * Quat(degToRad(90.0f), Vec3::yAxis) * Quat(degToRad(angl), Vec3::zAxis)).asMatrix()
			* Mat4::translation(cameraPos + cameraRot * (Vec3::forward + v)));
		m_itemModelShader.use();
		itemData.voxelizedItemModelsVao.bind();
		glDrawArrays(GL_TRIANGLES, itemInfo.model.vboVertexOffset, itemInfo.model.vertexCount);
	}

	drawCrosshair(screenSize);
}

void RenderingSystem::drawChunks(const ChunkSystem& chunkSystem, ShaderProgram& shader)
{
	chunkSystem.m_vao.bind();
	int drawn = 0;
	for (const auto& chunk : chunkSystem.m_chunksToDraw)
	{
		AB a{ Vec3(chunk->pos) * Chunk::SIZE + Vec3(Chunk::SIZE / 2.0f), Vec3(Chunk::SIZE) };
		if (m_frustum.collision(a))
		{
			drawn++;
			shader.setMat4("model", Mat4::translation(Vec3(chunk->pos) * Chunk::SIZE));
			glDrawArrays(GL_TRIANGLES, chunk->vboByteOffset / sizeof(uint32_t), chunk->vertexCount);
		}

		if (Debug::shouldShowChunkBorders)
		{
			Debug::drawCube(Vec3(chunk->pos) * Chunk::SIZE * Block::SIZE + Vec3(Chunk::SIZE) / 2.0, Vec3(Chunk::SIZE), Vec3(1, 1, 1));
		}
	}
	std::cout << "total: " << chunkSystem.m_chunksToDraw.size() << " drawn: " << drawn << '\n';

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

void RenderingSystem::shadowMapSetup()
{
	m_shadowMapTextures.bind();
	glTexImage3D(
		GL_TEXTURE_2D_ARRAY, 0, GL_DEPTH_COMPONENT32F, DEPTH_MAP_RESOLUTION, DEPTH_MAP_RESOLUTION, int(shadowCascadeLevels.size()) + 1,
		0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	constexpr float bordercolor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BORDER_COLOR, bordercolor);

	m_shadowMapFbo.bind();
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_shadowMapTextures.handle(), 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	m_shadowMapFbo.unbind();

	glGenBuffers(1, &matricesUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, matricesUBO);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(Mat4) * 16, nullptr, GL_STATIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, matricesUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void RenderingSystem::drawToShadowMap(const ChunkSystem& chunkSystem, const EntityManager& entityManager, const ItemData& itemData)
{
	m_chunkShadowShader.use();
	glActiveTexture(GL_TEXTURE0);
	chunkSystem.blockData.textureArray.bind();
	m_chunkShadowShader.setTexture("blockTextureArray", 0);
	m_shadowMapFbo.bind();
	glViewport(0, 0, DEPTH_MAP_RESOLUTION, DEPTH_MAP_RESOLUTION);
	glClear(GL_DEPTH_BUFFER_BIT);
	// Using GL_FRONT so only the back faces of the objects from the direction of the light can have shadows.
	glCullFace(GL_FRONT);
	drawChunks(chunkSystem, m_chunkShadowShader);
	//drawScene(chunkSystem, entityManager, itemData);
	m_shadowMapFbo.unbind();
}

void RenderingSystem::drawScene(const ChunkSystem& chunkSystem, const EntityManager& entityManager, const ItemData& itemData)
{
	// Only clearing depth buffer because the skybox is drawn.
	glCullFace(GL_BACK);
	glViewport(0, 0, m_screenSize.x, m_screenSize.y);
	glClear(GL_DEPTH_BUFFER_BIT);
	m_chunkShader.use();
	m_chunkShader.setMat4("projection", m_projection);
	m_chunkShader.setMat4("camera", m_view);
	m_chunkShader.setVec3("lightDir", m_directionalLightDir);
	m_chunkShader.setFloat("farPlane", m_farPlaneZ);
	m_chunkShader.setInt("cascadeCount", shadowCascadeLevels.size());
	for (size_t i = 0; i < shadowCascadeLevels.size(); ++i)
	{
		m_chunkShader.setFloat("cascadePlaneDistances[" + std::to_string(i) + "]", shadowCascadeLevels[i]);
	}
	glActiveTexture(GL_TEXTURE0);
	chunkSystem.blockData.textureArray.bind();
	m_chunkShader.setTexture("blockTextureArray", 0);
	glActiveTexture(GL_TEXTURE1);
	m_shadowMapTextures.bind();
	m_chunkShader.setTexture("shadowMap", 1);
	drawChunks(chunkSystem, m_chunkShader);

	m_itemModelShader.setMat4("projection", m_projection);
	m_itemModelShader.setMat4("view", m_view);
	m_itemBlockShader.setMat4("projection", m_projection);
	m_itemBlockShader.setMat4("view", m_view);
	for (const auto& [entity, itemComponent] : entityManager.getComponents<ItemComponent>())
	{
		const Vec3& pos = entityManager.getComponent<Position>(entity).value;

		float timeSinceSpawned = Time::currentTime() - itemComponent.spawnTime * 2.0f;

		const auto& itemInfo = itemData[itemComponent.item.item];
		if (itemInfo.isBlock)
		{
			m_itemBlockShader.setMat4("model",
				Quat(timeSinceSpawned, Vec3::yAxis).asMatrix()
				* Mat4::scale(Vec3(0.25f))
				* Mat4::translation(pos + Vec3(0.0f, (sin(timeSinceSpawned) + 1.0f) / 10.0f, 0.0)));

			const auto& blockInfo = chunkSystem.blockData[itemInfo.blockType];
			m_itemBlockShader.use();
			m_itemBlockShader.setUnsignedInt("faceTextureIndex[0]", blockInfo.frontTextureIndex);
			m_itemBlockShader.setUnsignedInt("faceTextureIndex[1]", blockInfo.backTextureIndex);
			m_itemBlockShader.setUnsignedInt("faceTextureIndex[2]", blockInfo.rightTextureIndex);
			m_itemBlockShader.setUnsignedInt("faceTextureIndex[3]", blockInfo.leftTextureIndex);
			m_itemBlockShader.setUnsignedInt("faceTextureIndex[4]", blockInfo.bottomTextureIndex);
			m_itemBlockShader.setUnsignedInt("faceTextureIndex[5]", blockInfo.topTextureIndex);
			m_cubeTrianglesVao.bind();
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		else
		{
			m_itemModelShader.setMat4("model",
				Quat(timeSinceSpawned, Vec3::yAxis).asMatrix()
				* Mat4::scale(Vec3(0.5f))
				* Mat4::translation(pos + Vec3(0.0f, (sin(timeSinceSpawned) + 1.0f) / 10.0f, 0.0)));
			m_itemModelShader.use();
			itemData.voxelizedItemModelsVao.bind();
			glDrawArrays(GL_TRIANGLES, itemInfo.model.vboVertexOffset, itemInfo.model.vertexCount);
		}
	}
}

void RenderingSystem::onScreenResize()
{
	m_fboTexture.bind();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_screenSize.x, m_screenSize.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	m_fboDepthTexture.bind();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_screenSize.x, m_screenSize.y, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
}

std::array<Vec3, 8> RenderingSystem::getFrustumCornersWorldSpace(const Mat4& proj, const Mat4& view)
{
	// NDC cube corners
	std::array<Vec3, 8> corners = {
		Vec3( 1,  1,  1),
		Vec3( 1, -1,  1),
		Vec3(-1,  1,  1),
		Vec3(-1, -1,  1),
		Vec3( 1,  1, -1),
		Vec3( 1, -1, -1),
		Vec3(-1,  1, -1),
		Vec3(-1, -1, -1),
	};

	const auto inverse = (view * proj).inverse();
	for (auto& corner : corners)
	{
		corner = inverse * corner;
	}

	return corners;
}

Mat4 RenderingSystem::getLightSpaceMatrix(float fov, float aspectRatio, const float nearPlane, const float farPlane, const Mat4& view, const Vec3& lightDir)
{
	const auto proj = Mat4::perspective(fov, aspectRatio, nearPlane, farPlane);
	const auto corners = getFrustumCornersWorldSpace(proj, view);

	Vec3 center(0);
	for (const auto& corner : corners)
		center += corner;
	center /= corners.size();

	const auto lightView = Mat4::lookAt(center - lightDir, center, Vec3::up);

	Vec3 min(std::numeric_limits<float>::min());
	Vec3 max(std::numeric_limits<float>::min());
	for (const auto& corner : corners)
	{
		const auto transformed = lightView * corner;
		min.x = std::min(min.x, transformed.x);
		max.x = std::max(max.x, transformed.x);
		min.y = std::min(min.y, transformed.y);
		max.y = std::max(max.y, transformed.y);
		min.z = std::min(min.z, transformed.z);
		max.z = std::max(max.z, transformed.z);
	}

	// Tune this parameter according to the scene
	constexpr float zMult = 20.0f;
	if (min.z < 0)
	{
		min.z *= zMult;
	}
	else
	{
		min.z /= zMult;
	}
	if (max.z < 0)
	{
		max.z /= zMult;
	}
	else
	{
		max.z *= zMult;
	}

	const Mat4 lightProjection = Mat4::orthographic(min, max);

	return lightView * lightProjection;
}

std::vector<Mat4> RenderingSystem::getLightSpaceMatrices(float fov, float aspectRatio, const Mat4& view, const Vec3& lightDir, float nearZ, float farZ, const std::vector<float>& shadowCascadeLevels)
{
	std::vector<Mat4> matrices;

	matrices.push_back(getLightSpaceMatrix(fov, aspectRatio, nearZ, shadowCascadeLevels[0], view, lightDir));
	for (size_t i = 1; i < shadowCascadeLevels.size(); i++)
	{
		matrices.push_back(getLightSpaceMatrix(fov, aspectRatio, shadowCascadeLevels[i - 1], shadowCascadeLevels[i], view, lightDir));
	}
	matrices.push_back(getLightSpaceMatrix(fov, aspectRatio, shadowCascadeLevels[shadowCascadeLevels.size() - 1], farZ, view, lightDir));

	return matrices;
}
