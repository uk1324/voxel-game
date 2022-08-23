#include <Game/Rendering/RenderingSystem.hpp>
#include <Game/Components/Position.hpp>
#include <Game/Components/Rotation.hpp>
#include <Game/Debug/Debug.hpp>
#include <Model/ModelLoader.hpp>
#include <Game/EntitySystem.hpp>
#include <Log/Log.hpp>
#include <Math/Interpolation.hpp>
#include <Game/Player/PlayerAnimationComponent.hpp>
#include <Math/Sphere.hpp>

#include <string>
#include <inttypes.h>
#include <imgui.h>

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
	, m_chunkWaterShader(SHADERS_PATH "Forward/chunkWater.vert", SHADERS_PATH "Forward/chunkWater.frag")
	, m_itemBlockShader(SHADERS_PATH "Objects/itemBlock.vert", SHADERS_PATH "Objects/itemBlock.frag")
	, m_itemModelShader(SHADERS_PATH "Objects/itemModel.vert", SHADERS_PATH "Objects/itemModel.frag")
	, m_modelShader(SHADERS_PATH "Objects/model.vert", SHADERS_PATH "Objects/model.frag")
	, m_blockParticleShader(SHADERS_PATH "Objects/blockParticle.vert", SHADERS_PATH "Objects/blockParticle.frag")

	, m_crosshairTexture("assets/textures/crosshair.png")
	, m_debugShader(SHADERS_PATH "Other/debugShapes.vert", SHADERS_PATH "Other/debugShapes.frag")

	, m_cubeLinesVbo(cubeLinesVertices, sizeof(cubeLinesVertices))
	, m_squareTrianglesVbo(squareTrianglesVertices, sizeof(squareTrianglesVertices))
	, m_pointVbo(point, sizeof(point))
	, m_LineVbo(lineData, sizeof(lineData))
	, m_squareTrianglesVbo2(squareTrianglesVertices2, sizeof(squareTrianglesVertices2))
	, m_cubeTrianglesVbo(cubeTriangleVertices, sizeof(cubeTriangleVertices))

	, m_squareShader(SHADERS_PATH "Other/2dTextured.vert", SHADERS_PATH "Other/2dTextured.frag")

	, m_geometryBufferFbo(Fbo::generate())
	, m_depthTexture(Texture::generate())
	, m_positionTexture(Texture::generate())
	, m_normalTexture(Texture::generate())
	, m_albedoTexture(Texture::generate())
	, m_defferedPassShader(SHADERS_PATH "Other/texturedQuad.vert", SHADERS_PATH "Deffered/deffered.frag")
	, m_defferedDrawNormals(SHADERS_PATH "Other/texturedQuad.vert", SHADERS_PATH "Deffered/Debug/normals.frag")
	, m_defferedDrawAlbedo(SHADERS_PATH "Other/texturedQuad.vert", SHADERS_PATH "Deffered/Debug/albedo.frag")
	, m_defferedDrawDepth(SHADERS_PATH "Other/texturedQuad.vert", SHADERS_PATH "Deffered/Debug/depth.frag")
	, m_texturedQuad(SHADERS_PATH "Other/texturedQuad.vert", SHADERS_PATH "Other/texturedQuad.frag")

	, m_postProcessFbo0(Fbo::generate())
	, m_postprocessTexture0(Texture::generate())
	, m_postprocessDepthTexture0(Texture::generate())
	, m_postProcessFbo1(Fbo::generate())
	, m_postprocessTexture1(Texture::generate())
	, m_postprocessDepthTexture1(Texture::generate())

	, m_postProcessShader(SHADERS_PATH "Other/texturedQuad.vert", SHADERS_PATH "PostProcess/postProcess.frag")
	, m_postProcessWaterShader(SHADERS_PATH "Other/texturedQuad.vert", SHADERS_PATH "PostProcess/waterPostProcess.frag")

	, m_shadowMapFbo(Fbo::generate())
	, m_cascadeZ({ 10, 30, 60.0f, 150 })
	, m_debugFbo(Fbo::generate())
	, m_debugTexture(Texture::generate())

	, m_selectedDefferedDrawItem(0)
	, m_enableShadowMapPass(true)
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
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthTexture.handle(), 0);
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_depthTexture.handle(), 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_depthTexture.handle(), 0);
	GLenum attachments[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, attachments);
	Fbo::unbind();


	for (size_t i = 0; i < m_cascadeZ.size() + 1; i++)
	{
		m_shadowMapTextures.push_back(Texture::generate());
		m_shadowMapTextures[i].bind();
		// Not sure if I should use nearest or linear filtering.
		/*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);*/
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
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

	auto initializePostProcessTexture = [](Fbo& fbo, Texture& color, Texture& depth)
	{
		fbo.bind();
		color.bind();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color.handle(), 0);
		depth.bind();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth.handle(), 0);
		Fbo::unbind();
	};
	initializePostProcessTexture(m_postProcessFbo0, m_postprocessTexture0, m_postprocessDepthTexture0);
	initializePostProcessTexture(m_postProcessFbo1, m_postprocessTexture1, m_postprocessDepthTexture1);


	m_blockParticleVao.bind();
	m_blockParticleVerticesVbo = Vbo(squareTrianglesVertices2, sizeof(squareTrianglesVertices2));
	m_blockParticleVerticesVbo.bind();
	Vao::setAttribute(0, BufferLayout(ShaderDataType::Float, 2, 0, sizeof(float) * 4, false));
	Vao::setAttribute(1, BufferLayout(ShaderDataType::Float, 2, sizeof(float) * 2, sizeof(float) * 4, false));
	m_blockParticleVbo = Vbo(sizeof(BlockParticleInstance) * MAX_PARTICLES_PER_INSTANCED_DRAW_CALL);
	m_blockParticleVbo.bind();
	Vao::setAttribute(2, ShaderDataType::Float, 4, false, sizeof(BlockParticleInstance), offsetof(BlockParticleInstance, model));
	Vao::setAttribute(3, ShaderDataType::Float, 4, false, sizeof(BlockParticleInstance), offsetof(BlockParticleInstance, model) + sizeof(float) * 4);
	Vao::setAttribute(4, ShaderDataType::Float, 4, false, sizeof(BlockParticleInstance), offsetof(BlockParticleInstance, model) + sizeof(float) * 8);
	Vao::setAttribute(5, ShaderDataType::Float, 4, false, sizeof(BlockParticleInstance), offsetof(BlockParticleInstance, model) + sizeof(float) * 12);
	Vao::setIntAttribute(6, ShaderDataType::UnsignedInt, 1, sizeof(BlockParticleInstance), offsetof(BlockParticleInstance, blockTextureIndex));
	glVertexAttribDivisor(2, 1);
	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
	glVertexAttribDivisor(6, 1); 
	// Set the uniforms at start.
	waterShaderConfig();
}

// Sometimes when the rendering happens the framebuffer is still incomplete.
// This is probably caused by this function not being called. 
// TODO: To fix this call this function with some screen size like 1x1 in the constructor.
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
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, m_screenSize.x, m_screenSize.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	// Also try GL_DEPTH32F_STENCIL8
	/*glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_screenSize.x, m_screenSize.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);*/
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_screenSize.x, m_screenSize.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	m_debugTexture.bind();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, m_screenSize.x, m_screenSize.y, 0, GL_RGB, GL_FLOAT, nullptr);

	m_postprocessTexture0.bind();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, m_screenSize.x, m_screenSize.y, 0, GL_RGB, GL_FLOAT, nullptr);
	m_postprocessDepthTexture0.bind();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, m_screenSize.x, m_screenSize.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

	m_postprocessTexture1.bind();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, m_screenSize.x, m_screenSize.y, 0, GL_RGB, GL_FLOAT, nullptr);
	m_postprocessDepthTexture1.bind();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, m_screenSize.x, m_screenSize.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

	glViewport(0, 0, m_screenSize.x, m_screenSize.y);
}

void RenderingSystem::update(
	const Vec2& screenSize,
	Entity playerEntity,
	const Vec3& cameraPos,
	const Quat& cameraRot, 
	EntityManager& entites,
	const ChunkSystem& chunkSystem, 
	const ItemData& itemData,
	const InputManager& input,
	const Opt<ItemStack>& heldItem)
{
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	if (m_screenSize != screenSize)
	{
		m_screenSize = screenSize;
		onScreenResize();
	}
	glDisable(GL_BLEND);
	static constexpr auto fov = degToRad(90.0f);
	const auto cameraDir = cameraRot * Vec3::forward;
	const auto aspectRatio = screenSize.x / screenSize.y;
	const auto projection = Mat4::perspective(fov, aspectRatio, m_nearPlaneZ, m_farPlaneZ);
	const auto view = Mat4::lookAt(cameraPos, cameraPos + cameraDir, Vec3::up);
	const auto worldToLightSpaceMats = 
		getShadowMatrices(fov, aspectRatio, projection, view, m_directionalLightDir, m_nearPlaneZ, m_farPlaneZ, m_cascadeZ);

	Frustum frustum(view * projection);

	auto drawToGeometryBuffer = [&]
	{
		// Use the same rotation for both the stencil and normal draw.
		const auto& playerAnimationComponent = entites.getComponent<PlayerAnimationComponent>(playerEntity);
		const auto swingProgress = playerAnimationComponent.swingProgress();
		float curve0 = sin(swingProgress * swingProgress * PI<float>); // Slow then fast
		float curve1 = sin(sqrt(swingProgress) * PI<float>); // Fast then slow
		const auto swingRotation =
			Quat(degToRad(curve0 * 20.0f), Vec3(Vec3::up))
			* Quat(degToRad(curve1 * 30.0f), Vec3(Vec3::left))
			* Quat(degToRad(curve1 * 80.0f), Vec3(Vec3::forward));
		const auto orientAndPutInFrontOfCamera =
			(cameraRot * Quat(degToRad(90.0f), Vec3::up)).asMatrix()
			* Mat4::translation(cameraPos + cameraDir);

		auto drawFirstPersonItem = [&]
		{
			const auto itemInfo = itemData[heldItem->item];

			if (itemInfo.isBlock)
			{
				static constexpr auto scale = 0.3;
				const auto model =
					Mat4::scale(Vec3(scale))
					* Quat(degToRad(45.0f), Vec3::up).asMatrix()
					* Mat4::translation(Vec3(0, 1 * scale, 0))
					* swingRotation.asMatrix()
					* Mat4::translation(Vec3(0, -1 * scale, 0))
					* Mat4::translation(Vec3(0.588, -0.405, -0.502))
					* orientAndPutInFrontOfCamera;

				const auto& blockInfo = chunkSystem.blockData[itemInfo.blockType];
				itemBlockShaderSetPerPass(view, projection);
				itemBlockDraw(blockInfo, model);
			}
			else
			{
				static constexpr auto scale = 0.5f;
				const auto model =
					Mat4::scale(Vec3(scale))
					* Quat(degToRad(32.393f), Vec3::forward).asMatrix()
					* Mat4::translation(Vec3(0, 1 * scale, 0))
					* swingRotation.asMatrix()
					* Mat4::translation(Vec3(0, -1 * scale, 0))
					* Mat4::translation(Vec3(0.588f, -0.275f, -0.580f))
					* orientAndPutInFrontOfCamera;

				m_itemModelShader.setMat4("model", model);
				m_itemModelShader.setMat4("view", view);
				m_itemModelShader.setMat4("projection", projection);
				m_itemModelShader.use();
				itemData.voxelizedItemModelsVao.bind();
				glDrawArrays(GL_TRIANGLES, itemInfo.model.vboVertexOffset, itemInfo.model.vertexCount);
			}
		};

		glViewport(0, 0, screenSize.x, screenSize.y);
		m_geometryBufferFbo.bind();

		// Draw the first person item to the stencil buffer then draw the scene everywere but inside the shape in the stencil buffer.
		// Then acutally draw the first person item.
		glClear(GL_STENCIL_BUFFER_BIT);
		glEnable(GL_STENCIL_TEST);
		glStencilMask(0xFF);
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		glDepthMask(GL_FALSE);
		glStencilFunc(GL_ALWAYS, 1, 0); // Always pass and write 1.
		glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE); // Always replace.

		if (heldItem.has_value())
		{
			drawFirstPersonItem();
		}
		
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		glDepthMask(GL_TRUE);
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP); // Never replace.
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF); // Only draw if not 1.

		drawScene(entites, itemData, chunkSystem, projection, view);

		m_blockParticleShader.setVec3("directionalLightDir", m_directionalLightDir);
		m_blockParticleShader.use();
		m_blockParticleVao.bind();
		m_blockParticleShader.setMat4("projection", projection);
		m_blockParticleShader.setMat4("view", view);
		m_blockParticleShader.setTexture("blockTextureArray", 0);
		glActiveTexture(GL_TEXTURE0);
		chunkSystem.blockData.textureArray.bind();
		glDisable(GL_CULL_FACE);
		size_t particlesToDrawInThisInstancedCall = 0;
		auto drawParticles = [this, &particlesToDrawInThisInstancedCall]
		{
			m_blockParticleVbo.bind(); 
			Vbo::setData(0, m_blockParticleInstances.data(), sizeof(BlockParticleInstance) * particlesToDrawInThisInstancedCall);
			//Vbo::allocateData(m_blockParticleInstances.data(), sizeof(BlockParticleInstance) * particlesToDrawInThisInstancedCall);
			glDrawArraysInstanced(GL_TRIANGLES, 0, 6, particlesToDrawInThisInstancedCall);
		};
		for (const auto& [particleEmmiterEntity, particleEmmiter] : entites.getComponents<BlockParticleEmmiter>())
		{
			const auto& pos = entites.getComponent<Position>(particleEmmiterEntity);
			const auto& blockInfo = chunkSystem.blockData[particleEmmiter.blockType];
			const auto textureIndex = blockInfo.isDecoration ? blockInfo.textureIndex : blockInfo.frontTextureIndex;
			for (const auto& particle : particleEmmiter.particles)
			{
				if (particlesToDrawInThisInstancedCall >= MAX_PARTICLES_PER_INSTANCED_DRAW_CALL)
				{
					drawParticles();
					particlesToDrawInThisInstancedCall = 0;
				}
				auto& particleInstance = m_blockParticleInstances[particlesToDrawInThisInstancedCall];
				particleInstance.model = Mat4::scale(Vec3(1.0f / 16.0f)) * cameraRot.asMatrix() * Mat4::translation(particle.pos);
				particleInstance.blockTextureIndex = textureIndex;
				particlesToDrawInThisInstancedCall++;
			}
		}
		if (particlesToDrawInThisInstancedCall > 0)
		{
			drawParticles();
		}

		glDisable(GL_STENCIL_TEST);
		if (heldItem.has_value())
		{
			drawFirstPersonItem();
		}



		Fbo::unbind(); 
	};
	
	auto drawToShadowMaps = [&]
	{
		m_shadowMapFbo.bind();
		glViewport(0, 0, SHADOW_MAP_SIZE, SHADOW_MAP_SIZE);
		for (size_t i = 0; i < m_shadowMapTextures.size(); i++)
		{
			m_shadowMapTextures[i].bind();
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_shadowMapTextures[i].handle(), 0);
			glCullFace(GL_FRONT);
			if (m_enableShadowMapPass)
			{
				drawScene(entites, itemData, chunkSystem, Mat4::identity, worldToLightSpaceMats[i].first);
			}
			else
			{
				glClear(GL_DEPTH_BUFFER_BIT);
			}
			glCullFace(GL_BACK);
		}
		Fbo::unbind();
	};

	auto setupDefferedFinal = [&]
	{
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
		m_defferedPassShader.setFloat("time", Time::currentTime());
		for (size_t i = 0; i < m_shadowMapTextures.size(); i++)
		{
			constexpr size_t START = 4;
			glActiveTexture(GL_TEXTURE0 + START + i);
			m_shadowMapTextures[i].bind();
			m_defferedPassShader.setTexture("shadowMaps[" + std::to_string(i) + "]", START + i);
		}
		for (size_t i = 0; i < worldToLightSpaceMats.size(); i++)
		{
			m_defferedPassShader.setMat4("worldToShadowMap[" + std::to_string(i) + "]", worldToLightSpaceMats[i].first);
		}
		for (size_t i = 0; i < m_cascadeZ.size(); i++)
		{
			m_defferedPassShader.setFloat("cascadeZ[" + std::to_string(i) + "]", m_cascadeZ[i]);
			m_defferedPassShader.setVec3("cascadeScale[" + std::to_string(i) + "]", worldToLightSpaceMats[i].second);
		}
		m_defferedPassShader.setInt("cascadeCount", m_cascadeZ.size());
		m_defferedPassShader.setFloat("farPlane", m_farPlaneZ);
		m_defferedPassShader.setMat4("worldToView", view);

		m_defferedPassShader.setMat4("viewProjection", view * projection);


		m_defferedPassShader.setVec3("directionalLightDir", m_directionalLightDir);
	};

	auto setupDefferedDrawNormals = [&]
	{
		m_defferedDrawNormals.use();
		glActiveTexture(GL_TEXTURE0);
		m_normalTexture.bind();
		m_defferedDrawNormals.setTexture("geometryNormals", 0);
	};

	auto setupDefferedDrawAlbedo = [&]
	{
		m_defferedDrawAlbedo.use();
		glActiveTexture(GL_TEXTURE0);
		m_albedoTexture.bind();
		m_defferedDrawAlbedo.setTexture("geometryAlbedo", 0);
	};

	auto setupDefferedDrawDepth = [&]
	{
		m_defferedDrawDepth.use();
		glActiveTexture(GL_TEXTURE0);
		m_depthTexture.bind();
		m_defferedDrawDepth.setTexture("geometryDepth", 0);
		m_defferedDrawDepth.setFloat("nearPlaneZ", m_nearPlaneZ);
		m_defferedDrawDepth.setFloat("farPlaneZ", m_farPlaneZ);
	};

	auto defferedDraw = [&]
	{
		glViewport(0, 0, screenSize.x, screenSize.y);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		m_squareTrianglesVao2.bind();
		glDrawArrays(GL_TRIANGLES, 0, 6);
		drawDebugShapes(projection, view);

		Renderer::drawSkybox(view, projection, m_skyboxData);
	};

	auto drawWater = [&]
	{
		auto drawWaterChunks = [&]
		{
			for (const auto& chunk : chunkSystem.m_chunksToDraw)
			{
				auto min = Vec3(chunk->pos) * Chunk::SIZE_V;
				auto max = min + Chunk::SIZE_V;
				if (frustum.intersects(Aabb(min, max)) == false)
					continue;

				m_chunkWaterShader.setMat4("model", Mat4::translation(Vec3(chunk->pos) * Chunk::SIZE_V));
				chunk->waterVao.bind();
				glDrawArrays(GL_TRIANGLES, 0, chunk->waterVertexCount);
			}
		};

		m_chunkWaterShader.use();
		m_chunkWaterShader.setMat4("projection", projection);
		m_chunkWaterShader.setMat4("camera", view);
		m_chunkWaterShader.setVec3("viewPos", cameraPos);
		m_chunkWaterShader.setFloat("time", Time::currentTime());

		// First draw depth so later only the closest transparent object is rendered.
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		glDepthFunc(GL_LESS);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_CULL_FACE);
		drawWaterChunks();
		glEnable(GL_CULL_FACE);
		glDisable(GL_BLEND);
		
		glDepthFunc(GL_LEQUAL);
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		glActiveTexture(GL_TEXTURE0);
		m_postprocessTexture0.bind();
		m_chunkWaterShader.setTexture("background", 0);
		m_chunkWaterShader.setVec2("screenSize", screenSize);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_CULL_FACE);
		drawWaterChunks();
		glEnable(GL_CULL_FACE);
		glDisable(GL_BLEND);
	};

	enum DefferedType
	{
		Final,
		Normals,
		Albedo,
		Depth,
		DefferedTypeCount
	};
	// C++ 17 doesn't support designated array initializers.
	const char* defferedTypeLabels[DefferedTypeCount];
	defferedTypeLabels[Final] = "Final";
	defferedTypeLabels[Normals] = "Normals";
	defferedTypeLabels[Albedo] = "Albedo";
	defferedTypeLabels[Depth] = "Depth";

	if (Debug::shouldDisplayRendererSettings)
	{
		ImGui::Begin("Renderer");
		if (ImGui::CollapsingHeader("Config"))
		{
			ImGui::Combo("##outputCombo", &m_selectedDefferedDrawItem, defferedTypeLabels, IM_ARRAYSIZE(defferedTypeLabels));
			ImGui::Checkbox("Shadow map pass", &m_enableShadowMapPass);
		}
		if (ImGui::CollapsingHeader("Frustum culling statistics across all passes"))
		{
			const auto percentCulled = [](size_t total, size_t drawn) -> float
			{
				if (total == 0)
					return 0;
				auto t = static_cast<float>(total);
				auto d = static_cast<float>(drawn);
				return (t - d) / t * 100;
			};
			ImGui::Text("chunks culled: %.2f%%", percentCulled(m_totalChunksToDraw, m_chunksDrawn));
			ImGui::Text("block items culled: %.2f%%", percentCulled(m_totalBlockItemsToDraw, m_blockItemsDrawn));
			ImGui::Text("voxelized texture items culled: %.2f%%", percentCulled(m_totalVoxelizedTextureItemsToDraw, m_voxelizedTextureItemsDrawn));
			ImGui::Text("meshes culled: %.2f%%", percentCulled(m_totalMeshesToDraw, m_meshesDrawn));
		}
		ImGui::End();
	}

	resetStatistics();

	drawToGeometryBuffer();
	drawToShadowMaps();

	switch (m_selectedDefferedDrawItem)
	{
	case Final: setupDefferedFinal(); break;
	case Normals: setupDefferedDrawNormals(); break;
	case Albedo: setupDefferedDrawAlbedo(); break;
	case Depth: setupDefferedDrawDepth(); break;
	}

	m_postProcessFbo0.bind();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	defferedDraw();

	const auto blockAtHead = chunkSystem.tryGetBlock(Vec3I(cameraPos.applied(floor)));
	const auto isUnderwater = blockAtHead.has_value() && (blockAtHead->type == BlockType::Water);
	if (isUnderwater)
	{
		m_postProcessFbo1.bind();
	}
	else
	{
		Fbo::unbind();
	}
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	m_postProcessShader.use();
	glActiveTexture(GL_TEXTURE0);
	m_postprocessTexture0.bind();
	m_postProcessShader.setTexture("inputColorTexture", 0);
	glActiveTexture(GL_TEXTURE1);
	m_postprocessDepthTexture0.bind();
	m_postProcessShader.setTexture("inputDepthTexture", 1);
	glViewport(0, 0, screenSize.x, screenSize.y);
	m_squareTrianglesVao2.bind();
	glDrawArrays(GL_TRIANGLES, 0, 6);
	drawWater();

	if (isUnderwater)
	{
		Fbo::unbind();
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		m_postProcessWaterShader.use();
		glActiveTexture(GL_TEXTURE0);
		m_postprocessTexture1.bind();
		m_postProcessWaterShader.setTexture("inputColorTexture", 0);
		glActiveTexture(GL_TEXTURE1);
		m_postprocessDepthTexture1.bind();
		m_postProcessWaterShader.setTexture("inputDepthTexture", 1);
		m_postProcessWaterShader.setVec3("waterColor", colorWater);
		m_postProcessWaterShader.setVec2("screenSize", screenSize);
		m_postProcessWaterShader.setMat4("inverseProjection", projection.inverse());
		glViewport(0, 0, screenSize.x, screenSize.y);
		m_squareTrianglesVao2.bind();
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
	

	//m_postProcessFbo0.bind();

	drawCrosshair(screenSize);
}

void RenderingSystem::drawScene(
	EntityManager& entityManager, 
	const ItemData& itemData,
	const ChunkSystem& chunkSystem, 
	const Mat4& projection, 
	const Mat4& view)
{
	// TODO: Disable blend for faster rendering.
	glDisable(GL_BLEND);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	Frustum frustum(view * projection);

	auto drawChunks = [&]
	{
		m_chunkShader.use();
		m_chunkShader.setMat4("projection", projection);
		m_chunkShader.setMat4("camera", view);
		glActiveTexture(GL_TEXTURE0);
		chunkSystem.blockData.textureArray.bind();
		m_chunkShader.setTexture("blockTextureArray", 0);
		chunkSystem.m_vao.bind();
		m_totalChunksToDraw += chunkSystem.m_chunksToDraw.size();
		for (const auto& chunk : chunkSystem.m_chunksToDraw)
		{
			auto min = Vec3(chunk->pos) * Chunk::SIZE_V;
			auto max = min + Chunk::SIZE_V;
			if (frustum.intersects(Aabb(min, max)) == false)
				continue;

			m_chunksDrawn++;
			m_chunkShader.setMat4("model", Mat4::translation(Vec3(chunk->pos) * Chunk::SIZE_V));
			glDrawArrays(GL_TRIANGLES, chunk->vboByteOffset / sizeof(uint32_t), chunk->vertexCount);

			if (Debug::shouldShowChunkBorders)
			{
				Debug::drawCube(Vec3(chunk->pos) * Chunk::SIZE_V * Block::SIZE + Chunk::SIZE_V / 2.0, Chunk::SIZE_V, Vec3(1, 1, 1));
			}
		}
	};

	auto drawItems = [&]
	{
		m_itemModelShader.setMat4("projection", projection);
		m_itemModelShader.setMat4("view", view);
		m_itemBlockShader.setMat4("projection", projection);
		m_itemBlockShader.setMat4("view", view);
		for (const auto& [entity, itemComponent] : entityManager.getComponents<ItemComponent>())
		{
			const Vec3& pos = entityManager.getComponent<Position>(entity).value;
			const auto boxHalfSize = Vec3(0.6);
			const auto boundingBox = Aabb(pos - boxHalfSize, pos + boxHalfSize);

			float timeSinceSpawned = Time::currentTime() - itemComponent.spawnTime * 2.0f;

			const auto& itemInfo = itemData[itemComponent.item.item];
			if (itemInfo.isBlock)
			{
				m_totalBlockItemsToDraw++;
				if (frustum.intersects(boundingBox) == false)
					continue;
				m_blockItemsDrawn++;

				const auto model = 
					Quat(timeSinceSpawned, Vec3::yAxis).asMatrix()
					* Mat4::scale(Vec3(0.25f))
					* Mat4::translation(pos + Vec3(0.0f, (sin(timeSinceSpawned) + 1.0f) / 10.0f, 0.0));

				const auto& blockInfo = chunkSystem.blockData[itemInfo.blockType];
				itemBlockShaderSetPerPass(view, projection);
				itemBlockDraw(blockInfo, model);
			}
			else
			{
				m_totalBlockItemsToDraw++;
				if (frustum.intersects(boundingBox) == false)
					continue;
				m_blockItemsDrawn++;

				m_itemModelShader.setMat4("model",
					Quat(timeSinceSpawned, Vec3::yAxis).asMatrix()
					* Mat4::scale(Vec3(0.5f))
					* Mat4::translation(pos + Vec3(0.0f, (sin(timeSinceSpawned) + 1.0f) / 10.0f, 0.0)));
				m_itemModelShader.use();
				itemData.voxelizedItemModelsVao.bind();
				glDrawArrays(GL_TRIANGLES, itemInfo.model.vboVertexOffset, itemInfo.model.vertexCount);
			}
		}
	};

	auto drawMeshes = [&]
	{
		m_modelShader.setMat4("view", view);
		m_modelShader.setMat4("projection", projection);
		for (auto& [entity, model] : entityManager.getComponents<AnimatedModel>())
		{
			const Vec3& pos = entityManager.getComponent<Position>(entity).value;
			const Quat& rotation = entityManager.getComponent<Rotation>(entity).value;
			const auto offset = Vec3(0, -0.15f, 0);
			const auto boundingSphere = Sphere(pos + offset, model.model.boundingSphereRadius);
			m_totalMeshesToDraw++;
			if (frustum.intersects(boundingSphere) == false)
				continue;
			m_meshesDrawn++;

			model.update();

			m_modelShader.use();
			m_modelShader.setMat4("model", rotation.asMatrix() * Mat4::translation(pos + offset));
			glActiveTexture(GL_TEXTURE0);
			model.model.textures[1].bind();
			m_modelShader.setTexture("textureSampler", 0);
			glActiveTexture(GL_TEXTURE1);
			model.model.textures[0].bind();
			m_modelShader.setTexture("normalMap", 1);
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
	};

	drawChunks();
	drawItems();
	drawMeshes();
}

void RenderingSystem::resetStatistics()
{
	m_totalChunksToDraw = 0;
	m_chunksDrawn = 0;
	m_totalBlockItemsToDraw = 0;
	m_blockItemsDrawn = 0;
	m_totalVoxelizedTextureItemsToDraw = 0;
	m_voxelizedTextureItemsDrawn = 0;
	m_totalMeshesToDraw = 0;
	m_meshesDrawn = 0;
}

std::pair<Mat4, Vec3> RenderingSystem::getShadowMatrix(float fov, float aspectRatio, const float cascadeNearZ, const float cascadeFarZ, const Mat4& proj, const Mat4& view, const Vec3& lightDir)
{
	const auto projection = Mat4::perspective(fov, aspectRatio, cascadeNearZ, cascadeFarZ);
	const auto viewProjectionInverse = (view * projection).inverse();

	std::array<Vec3, 8> frustumCornersWorldSpace = {
		Vec3(-1.0f, 1.0f, -1.0f),
		Vec3(1.0f, 1.0f, -1.0f),
		Vec3(1.0f, -1.0f, -1.0f),
		Vec3(-1.0f, -1.0f, -1.0f),
		Vec3(-1.0f, 1.0f, 1.0f),
		Vec3(1.0f, 1.0f, 1.0f),
		Vec3(1.0f, -1.0f, 1.0f),
		Vec3(-1.0f, -1.0f, 1.0f),
	};
	for (auto& corner : frustumCornersWorldSpace)
		corner = viewProjectionInverse * corner;

	Vec3 frustumCenter(0.0f);
	for (const auto& corner : frustumCornersWorldSpace)
		frustumCenter += corner;
	frustumCenter /= frustumCornersWorldSpace.size();

	float boundingSphereRadius = 0.0f;
	for (const auto& corner : frustumCornersWorldSpace)
	{
		float distance = (corner - frustumCenter).length();
		boundingSphereRadius = std::max(boundingSphereRadius, distance);
	}
	boundingSphereRadius = std::ceil(boundingSphereRadius * 16.0f) / 16.0f;

	const Vec3 max(boundingSphereRadius, boundingSphereRadius, boundingSphereRadius);
	const auto min = -max;
	const auto cascadeExtents = max - min;

	const auto shadowCameraPos = frustumCenter + m_directionalLightDir * min.z;
	const auto shadowView = Mat4::lookAt(shadowCameraPos, frustumCenter, Vec3::up);

	/*auto shadowProjection = Mat4::orthographic(Vec3(min.x, min.y, -cascadeExtents.z), Vec3(max.x, max.y, cascadeExtents.z));*/
	auto shadowProjection = Mat4::orthographic(Vec3(min.x, min.y, -Chunk::SIZE_Y), Vec3(max.x, max.y, cascadeExtents.z));
	const auto shadowMatrix = shadowView * shadowProjection;
	const auto shadowOrigin = (shadowMatrix * Vec3(0.0f)) * (SHADOW_MAP_SIZE / 2.0f);

	const auto roundedOrigin = shadowOrigin.applied(floor);
	const auto roundOffset = (roundedOrigin - shadowOrigin) * (2.0f / SHADOW_MAP_SIZE);

	shadowProjection(3, 0) += roundOffset.x;
	shadowProjection(3, 1) += roundOffset.y;

	const auto texelAlignedShadowMatrix = (shadowView * shadowProjection);

	return { texelAlignedShadowMatrix, Vec3(1.0f / cascadeExtents.x, 1.0f / cascadeExtents.y, 1.0f / cascadeExtents.z * 2) };
}

std::vector<std::pair<Mat4, Vec3>> RenderingSystem::getShadowMatrices(float fov, float aspectRatio, const Mat4& proj, const Mat4& view, const Vec3& lightDir, float nearZ, float farZ, const std::vector<float>& cascadeZ)
{
	std::vector<std::pair<Mat4, Vec3>> matrices;
	for (size_t i = 0; i < cascadeZ.size() + 1; ++i)
	{
		if (i == 0)
		{
			matrices.push_back(getShadowMatrix(fov, aspectRatio, nearZ, cascadeZ[i], proj, view, lightDir));
		}
		else if (i < cascadeZ.size())
		{
			matrices.push_back(getShadowMatrix(fov, aspectRatio, cascadeZ[i - 1], cascadeZ[i], proj, view, lightDir));
		}
		else
		{
			matrices.push_back(getShadowMatrix(fov, aspectRatio, cascadeZ[i - 1], farZ, proj, view, lightDir));
		}
	}
	return matrices;
}

void RenderingSystem::itemBlockShaderSetPerPass(const Mat4& view, const Mat4& projection)
{
	m_itemBlockShader.setMat4("view", view);
	m_itemBlockShader.setMat4("projection", projection);
}

void RenderingSystem::itemBlockDraw(const BlockData::Entry& blockInfo, const Mat4& model)
{
	// Whether these should be set per pass or draw depends on the usage.
	m_itemBlockShader.use();
	m_cubeTrianglesVao.bind();

	m_itemBlockShader.setUnsignedInt("faceTextureIndex[0]", blockInfo.frontTextureIndex);
	m_itemBlockShader.setUnsignedInt("faceTextureIndex[1]", blockInfo.backTextureIndex);
	m_itemBlockShader.setUnsignedInt("faceTextureIndex[2]", blockInfo.rightTextureIndex);
	m_itemBlockShader.setUnsignedInt("faceTextureIndex[3]", blockInfo.leftTextureIndex);
	m_itemBlockShader.setUnsignedInt("faceTextureIndex[4]", blockInfo.bottomTextureIndex);
	m_itemBlockShader.setUnsignedInt("faceTextureIndex[5]", blockInfo.topTextureIndex);
	m_itemBlockShader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

void RenderingSystem::waterShaderConfig()
{
	using namespace ImGui;
	//Begin("water");

	//SliderInt("octaves", &octaves, 1, 6);
	//SliderFloat("persistence", &persistence, 0.1, 2);
	//SliderFloat("diagonalNormalOffset", &diagonalNormalOffset, 0.1, 5);
	//SliderFloat("inputNoiseScale", &inputNoiseScale, 0.05, 2);
	//SliderInt("specularIntensity", &specularIntensity, 1, 256);
	//SliderFloat("diagonalScale", &diagonalScale, 0.1, 2);
	//SliderFloat("diagonalNoiseScale", &diagonalNoiseScale, 0.1, 2);
	//ColorPicker3("colorWater", colorWater.data());
	//ColorPicker3("colorSpecular", colorSpecular.data());
	//SliderFloat("timeScale", &timeScale, 0.1, 10);

	//End();

	m_chunkWaterShader.setInt("octaves", octaves);
	m_chunkWaterShader.setFloat("persistence", persistence);
	m_chunkWaterShader.setFloat("diagonalNormalOffset", diagonalNormalOffset);
	m_chunkWaterShader.setFloat("inputNoiseScale", inputNoiseScale);
	m_chunkWaterShader.setInt("specularIntensity", specularIntensity);
	m_chunkWaterShader.setFloat("diagonalScale", diagonalScale);
	m_chunkWaterShader.setFloat("diagonalNoiseScale", diagonalNoiseScale);
	m_chunkWaterShader.setVec3("colorWater", colorWater);
	m_chunkWaterShader.setVec3("colorSpecular", colorSpecular);
	m_chunkWaterShader.setFloat("timeScale", timeScale);
}

void RenderingSystem::drawTexturedQuad(Texture& texture)
{
	glClear(GL_DEPTH_BUFFER_BIT);
	m_squareTrianglesVao2.bind();
	glActiveTexture(GL_TEXTURE0);
	texture.bind();
	m_texturedQuad.use();
	m_texturedQuad.setTexture("texture", 0);
	glDrawArrays(GL_TRIANGLES, 0, 6);
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
	glDepthFunc(GL_LEQUAL);
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