#include <Game/Rendering/RenderingSystem.hpp>
#include <Game/Components/Position.hpp>
#include <Game/Components/Rotation.hpp>
#include <Game/Debug/Debug.hpp>
#include <Model/ModelLoader.hpp>
#include <Game/EntitySystem.hpp>
#include <Log/Log.hpp>
#include <Math/Interpolation.hpp>

#include <string>
#include <inttypes.h>

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

//// Draw skeleton with propagate transform in the model loader.
//static void draw(const Vec3& pos, Model::Node& node, const Vec3& old)
//{
//	Vec3 transformed = node.output * Vec3(0) + old;
//	Debug::drawLine(pos, transformed);
//	for (const auto& child : node.children)
//	{
//		draw(transformed, *child, old);
//		Debug::drawPoint(transformed);
//	}
//}
//
//static void drawSkeleton(const Vec3& translation, Model::Node& node, const Mat4& parentTransform, const Vec3& parentTransformed)
//{
//	const Mat4 thisTransform = node.output * parentTransform;
//	const Vec3 transformed = thisTransform * Vec3(0) + translation;
//	Debug::drawLine(parentTransformed, transformed);
//	for (const auto& child : node.children)
//	{
//		drawSkeleton(translation, *child, thisTransform, transformed);
//		Debug::drawPoint(transformed);
//	}
//}

#define SHADERS_PATH "src/Game/Rendering/Shaders/"

#include <iostream>

RenderingSystem::RenderingSystem(Scene& scene)
	: m_skyboxData(
		"assets/textures/skybox/right.png",
		"assets/textures/skybox/left.png",
		"assets/textures/skybox/top.png",
		"assets/textures/skybox/bottom.png",
		"assets/textures/skybox/front.png",
		"assets/textures/skybox/back.png")

	, m_chunkShader(SHADERS_PATH "Objects/chunk.vert", SHADERS_PATH "Objects/chunk.frag")
	, m_itemBlockShader(SHADERS_PATH "Objects/itemBlock.vert", SHADERS_PATH "Objects/itemBlock.frag")
	, m_itemModelShader(SHADERS_PATH "Objects/itemModel.vert", SHADERS_PATH "Objects/itemModel.frag")
	, m_modelShader(SHADERS_PATH "Objects/model.vert", SHADERS_PATH "Objects/model.frag")

	, m_crosshairTexture("assets/textures/crosshair.png")
	//, m_squareShader("src/Game/Rendering/Shaders/2dTextured.vert", "src/Game/Rendering/Shaders/2dTextured.frag")
	, m_debugShader(SHADERS_PATH "Other/debugShapes.vert", SHADERS_PATH "Other/debugShapes.frag")
	//, m_texturedSquareShader("src/Game/Inventory/Shaders/uiTextured.vert", "src/Game/Rendering/Shaders/debugDepth.frag")

	, m_cubeLinesVbo(cubeLinesVertices, sizeof(cubeLinesVertices))
	, m_squareTrianglesVbo(squareTrianglesVertices, sizeof(squareTrianglesVertices))
	, m_pointVbo(point, sizeof(point))
	, m_LineVbo(lineData, sizeof(lineData))
	, m_squareTrianglesVbo2(squareTrianglesVertices2, sizeof(squareTrianglesVertices2))
	, m_cubeTrianglesVbo(cubeTriangleVertices, sizeof(cubeTriangleVertices))

	, m_geometryBufferFbo(Fbo::generate())
	, m_depthTexture(Texture::generate())
	, m_positionTexture(Texture::generate())
	, m_normalTexture(Texture::generate())
	, m_albedoTexture(Texture::generate())
	, m_defferedPassShader("src/Game/Rendering/Shaders/Deffered/deffered.vert", "src/Game/Rendering/Shaders/Deffered/deffered.frag")

	, m_directionalLightDir(Vec3(-0.5, -1, -0.5).normalized())

	, m_shadowMapFbo(Fbo::generate())
	, m_cascadeZ({ 10, 30, 50, 60 })
	, m_debugFbo(Fbo::generate())
	, m_debugTexture(Texture::generate())
{
	m_cubeLinesVao.bind();
	m_cubeLinesVbo.bind();
	Vao::setAttribute(0, BufferLayout(ShaderDataType::Float, 3, 0, sizeof(float) * 3, false));
	m_pointVao.bind();
	m_pointVbo.bind();
	Vao::setAttribute(0, BufferLayout(ShaderDataType::Float, 3, 0, sizeof(float) * 3, false));
	m_LineVao.bind();
	m_LineVbo.bind();
	Vao::setAttribute(0, BufferLayout(ShaderDataType::Float, 3, 0, sizeof(float) * 3, false));
	m_squareTrianglesVao.bind();
	m_squareTrianglesVbo.bind();
	Vao::setAttribute(0, BufferLayout(ShaderDataType::Float, 2, 0, sizeof(float) * 2, false));
	m_squareTrianglesVao2.bind();
	m_squareTrianglesVbo2.bind();
	Vao::setAttribute(0, BufferLayout(ShaderDataType::Float, 2, 0, sizeof(float) * 4, false));
	Vao::setAttribute(1, BufferLayout(ShaderDataType::Float, 2, sizeof(float) * 2, sizeof(float) * 4, false));

	m_cubeTrianglesVao.bind();
	m_cubeTrianglesVbo.bind();
	Vao::setAttribute(0, BufferLayout(ShaderDataType::Float, 3, 0, sizeof(float) * 6, false));
	Vao::setAttribute(1, BufferLayout(ShaderDataType::Float, 2, sizeof(float) * 3, sizeof(float) * 6, false));
	Vao::setAttribute(2, BufferLayout(ShaderDataType::Float, 1, sizeof(float) * 5, sizeof(float) * 6, false));

	m_geometryBufferFbo.bind();

	auto configGeometryBufferTexture = [](Texture& texture, size_t attachmentIndex)
	{
		texture.bind();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + attachmentIndex, GL_TEXTURE_2D, texture.handle(), 0);
	};

	configGeometryBufferTexture(m_positionTexture, 0);
	configGeometryBufferTexture(m_normalTexture, 1);
	configGeometryBufferTexture(m_albedoTexture, 2);
	m_depthTexture.bind();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthTexture.handle(), 0);
	GLenum attachments[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, attachments);
	Fbo::unbind();


	for (size_t i = 0; i < m_cascadeZ.size() + 1; i++)
	{
		m_shadowMapTextures.push_back(Texture::generate());
		m_shadowMapTextures[i].bind();
		// Not sure if I should use nearest or linear filtering.
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		// Have to use a floating point texture format for shadows to work.
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, SHADOW_MAP_SIZE, SHADOW_MAP_SIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	}

	m_shadowMapFbo.bind();
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	Fbo::unbind();

	m_debugFbo.bind();
	m_debugTexture.bind();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_debugTexture.handle(), 0);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	Fbo::unbind();
}

#include <imgui.h>

void RenderingSystem::onScreenResize()
{
	// When outputting from a fragment shader the texture format doesn't matter. Every output has to be a vec4.
	m_positionTexture.bind();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, m_screenSize.x, m_screenSize.y, 0, GL_RGB, GL_FLOAT, nullptr);

	m_normalTexture.bind();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, m_screenSize.x, m_screenSize.y, 0, GL_RGB, GL_FLOAT, nullptr);

	m_albedoTexture.bind();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, m_screenSize.x, m_screenSize.y, 0, GL_RGB, GL_FLOAT, nullptr);

	m_depthTexture.bind();
	// Maybe use GL_DEPTH24_STENCIL8 - better driver compatiblity apparently.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, m_screenSize.x, m_screenSize.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

	m_debugTexture.bind();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, m_screenSize.x, m_screenSize.y, 0, GL_RGB, GL_FLOAT, nullptr);

	glViewport(0, 0, m_screenSize.x, m_screenSize.y);
}

void RenderingSystem::update(
	const Vec2& screenSize, 
	const Vec3& cameraPos,
	const Quat& cameraRot, 
	EntityManager& entites,
	const ChunkSystem& chunkSystem, 
	const ItemData& itemData)
{
	if (m_screenSize != screenSize)
	{
		m_screenSize = screenSize;
		onScreenResize();
	}

	static constexpr auto fov = degToRad(90.0f);
	const auto cameraDir = cameraRot * Vec3::forward;
	const auto aspectRatio = screenSize.x / screenSize.y;
	const auto projection = Mat4::perspective(fov, aspectRatio, m_nearPlaneZ, m_farPlaneZ);
	const auto view = Mat4::lookAt(cameraPos, cameraPos + cameraDir, Vec3::up);


	m_geometryBufferFbo.bind();
	drawScene(entites, itemData, chunkSystem, projection, view);
	Renderer::drawSkybox(view, projection, m_skyboxData);
	drawDebugShapes(projection, view);
	glFrontFace(GL_CW);
	Fbo::unbind();
	
	m_shadowMapFbo.bind();
	auto ms = getLightSpaceMatrices(fov, aspectRatio, view, m_directionalLightDir, m_nearPlaneZ, m_farPlaneZ, m_cascadeZ);
	glViewport(0, 0, SHADOW_MAP_SIZE, SHADOW_MAP_SIZE);
	for (size_t i = 0; i < m_shadowMapTextures.size(); i++)
	{
		m_shadowMapTextures[i].bind();
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_shadowMapTextures[i].handle(), 0);
		glCullFace(GL_FRONT);
		drawScene(entites, itemData, chunkSystem, Mat4::identity, ms[i]);
		glCullFace(GL_BACK);
	}
	//glDisable(GL_POLYGON_OFFSET_FILL);
	glViewport(0, 0, screenSize.x, screenSize.y);
	Fbo::unbind();

	m_defferedPassShader.use();
	glActiveTexture(GL_TEXTURE0);
	m_positionTexture.bind();
	m_defferedPassShader.setTexture("geometryPosWorld", 0);
	glActiveTexture(GL_TEXTURE1);
	m_albedoTexture.bind();
	m_defferedPassShader.setTexture("geometryAlbedo", 1);
	glActiveTexture(GL_TEXTURE2);
	m_normalTexture.bind();
	m_defferedPassShader.setTexture("geometryNormal", 2);
	glActiveTexture(GL_TEXTURE3);
	m_depthTexture.bind();
	m_defferedPassShader.setTexture("geometryDepth", 3);
	for (size_t i = 0; i < m_shadowMapTextures.size(); i++)
	{
		constexpr size_t START = 4;
		glActiveTexture(GL_TEXTURE0 + START + i);
		m_shadowMapTextures[i].bind();
		m_defferedPassShader.setTexture("shadowMaps[" + std::to_string(i) + "]", START + i);
	}
	for (size_t i = 0; i < ms.size(); i++)
	{
		m_defferedPassShader.setMat4("worldToShadowMap[" + std::to_string(i) + "]", ms[i]);
	}
	for (size_t i = 0; i < m_cascadeZ.size(); i++)
	{
		m_defferedPassShader.setFloat("cascadeZ[" + std::to_string(i) + "]", m_cascadeZ[i]);
	}
	m_defferedPassShader.setInt("cascadeCount", m_cascadeZ.size());
	m_defferedPassShader.setFloat("farPlane", m_farPlaneZ);
	m_defferedPassShader.setMat4("worldToView", view);
	m_defferedPassShader.setVec3("directionalLightDir", m_directionalLightDir);

	m_squareTrianglesVao2.bind();
	glDrawArrays(GL_TRIANGLES, 0, 6);
	Renderer::drawSkybox(view, projection, m_skyboxData);

	Fbo::unbind();
	glViewport(0, 0, screenSize.x, screenSize.y);
}

void RenderingSystem::drawScene(
	EntityManager& entityManager, 
	const ItemData& itemData,
	const ChunkSystem& chunkSystem, 
	const Mat4& projection, 
	const Mat4& view)
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	m_chunkShader.use();
	m_chunkShader.setMat4("projection", projection);
	m_chunkShader.setMat4("camera", view);
	glActiveTexture(GL_TEXTURE0);
	chunkSystem.blockData.textureArray.bind();
	m_chunkShader.setTexture("blockTextureArray", 0);
	chunkSystem.m_vao.bind();
	Frustum frustum(view * projection);
	int drawn = 0;
	for (const auto& chunk : chunkSystem.m_chunksToDraw)
	{
		auto min = Vec3(chunk->pos) * Chunk::SIZE_V;
		auto max = min + Chunk::SIZE_V;
		if (frustum.intersects(Aabb(min, max)))
		{
			drawn++;
			m_chunkShader.setMat4("model", Mat4::translation(Vec3(chunk->pos) * Chunk::SIZE_V));
			glDrawArrays(GL_TRIANGLES, chunk->vboByteOffset / sizeof(uint32_t), chunk->vertexCount);
		}

		if (Debug::shouldShowChunkBorders)
		{
			Debug::drawCube(Vec3(chunk->pos) * Chunk::SIZE_V * Block::SIZE + Chunk::SIZE_V / 2.0, Chunk::SIZE_V, Vec3(1, 1, 1));
		}
	}
	//std::cout << "draw " << drawn << " / " << chunkSystem.m_chunksToDraw.size() << '\n';

	m_itemModelShader.setMat4("projection", projection);
	m_itemModelShader.setMat4("view", view);
	m_itemBlockShader.setMat4("projection", projection);
	m_itemBlockShader.setMat4("view", view);
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

	m_modelShader.setMat4("view", view);
	m_modelShader.setMat4("projection", projection);
	for (auto& [entity, model] : entityManager.getComponents<AnimatedModel>())
	{
		const Vec3& pos = entityManager.getComponent<Position>(entity).value;
		const Quat& rotation = entityManager.getComponent<Rotation>(entity).value;
		model.update();

		m_modelShader.use();
		m_modelShader.setMat4("model", rotation.asMatrix() * Mat4::translation(pos - Vec3(0, 0.15f, 0)));
		glActiveTexture(GL_TEXTURE0);
		model.model.textures[1].bind();
		m_modelShader.setInt("textureSampler", 0);
		model.model.meshes[0].vao.bind();
		model.model.buffers[0].bindAsIndexBuffer();

		for (size_t i = 0; i < model.model.joints.size(); i++)
		{
			m_modelShader.setMat4(
				"jointMatrices[" + std::to_string(i) + "]", 
				model.model.inverseBindMatrices[i] * model.nodeTransforms[model.model.nodeIndex(model.model.joints[i])]);
			// First the inverseBindMatrix is applied which undoes the bind pose translating every joint (not vertex) to the zero vector
			// in object space. Then the transform is applied putting the mesh in the animated pose.
		}
		glFrontFace(GL_CCW);
		glDrawElements(
			GL_TRIANGLES, 
			model.model.meshes[0].indicesCount, 
			static_cast<GLenum>(model.model.meshes[0].indexType), 
			reinterpret_cast<void*>(model.model.meshes[0].indicesByteOffset));
		glFrontFace(GL_CW);
	}
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

	// Create an AABB of frustum from the light's view.
	Vec3 min(std::numeric_limits<float>::infinity());
	Vec3 max(-std::numeric_limits<float>::infinity());
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

	// Maybe just set the max.z of the non transformed AABB to max height.
	// Also include geometry the is behind the camera because it can also cast shadows.
	constexpr float zMult = 160.0f;
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

std::vector<Mat4> RenderingSystem::getLightSpaceMatrices(float fov, float aspectRatio, const Mat4& view, const Vec3& lightDir, float nearZ, float farZ, const std::vector<float>& cascadeZ)
{
	std::vector<Mat4> matrices;
	for (size_t i = 0; i < cascadeZ.size() + 1; ++i)
	{
		if (i == 0)
		{
			matrices.push_back(getLightSpaceMatrix(fov, aspectRatio, nearZ, cascadeZ[i], view, lightDir));
		}
		else if (i < cascadeZ.size())
		{
			matrices.push_back(getLightSpaceMatrix(fov, aspectRatio, cascadeZ[i - 1], cascadeZ[i], view, lightDir));
		}
		else
		{
			matrices.push_back(getLightSpaceMatrix(fov, aspectRatio, cascadeZ[i - 1], farZ, view, lightDir));
		}
	}
	return matrices;
}

std::array<Vec3, 8> RenderingSystem::getFrustumCornersWorldSpace(const Mat4& proj, const Mat4& view)
{
	// NDC cube corners
	std::array<Vec3, 8> corners = {
		Vec3(1,  1,  1),
		Vec3(1, -1,  1),
		Vec3(-1,  1,  1),
		Vec3(-1, -1,  1),
		Vec3(1,  1, -1),
		Vec3(1, -1, -1),
		Vec3(-1,  1, -1),
		Vec3(-1, -1, -1),
	};
	// To projection matrix projects the frustum into the NDC cube.
	// Doing the inverse will give the frustum coodinates.
	// There are faster ways to do this like calculating it based on camera position, rotation and near and far plane distances.
	const auto inverse = (view * proj).inverse();
	for (auto& corner : corners)
	{
		corner = inverse * corner;
	}
	return corners;
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