#include <Game/Rendering/RenderingSystem.hpp>
#include <Game/Components/Position.hpp>
#include <Game/Debug/Debug.hpp>
#include <Model/ModelLoader.hpp>
#include <Game/EntitySystem.hpp>
#include <Log/Log.hpp>

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

std::vector<Vec3> getFrustumCornersWorldSpace(const Mat4& proj, const Mat4& view)
{
	const auto inv = (view * proj).inverse();

	std::vector<Vec3> frustumCorners;
	for (unsigned int x = 0; x < 2; ++x)
	{
		for (unsigned int y = 0; y < 2; ++y)
		{
			for (unsigned int z = 0; z < 2; ++z)
			{
				//auto v = Vec3(2.0f * x - 1.0f, 2.0f * y - 1.0f, 2.0f * z - 1.0f);
				//std::cout << v.x << v.y << v.z << '\n';
				const Vec3 pt = inv * Vec3(2.0f * x - 1.0f, 2.0f * y - 1.0f, 2.0f * z - 1.0f);
				//frustumCorners.push_back(pt / pt.w);
				frustumCorners.push_back(pt);
			}
		}
	}

	return frustumCorners;
}

Mat4 getLightSpaceMatrix(const float nearPlane, const float farPlane, float fov, float aspectRatio, const Mat4& view, Vec3 lightDir)
{
	const auto proj = Mat4::perspective(fov, aspectRatio, nearPlane, farPlane);
	const auto corners = getFrustumCornersWorldSpace(proj, view);

	Vec3 center(0);
	for (const auto& v : corners)
	{
		center += v;
	}
	center /= corners.size();

	const auto lightView = Mat4::lookAt(center + lightDir, center, Vec3(0.0f, 1.0f, 0.0f));

	float minX = std::numeric_limits<float>::infinity();
	float maxX = -std::numeric_limits<float>::infinity();
	float minY = std::numeric_limits<float>::infinity();
	float maxY = -std::numeric_limits<float>::infinity();
	float minZ = std::numeric_limits<float>::infinity();
	float maxZ = -std::numeric_limits<float>::infinity();
	for (const auto& v : corners)
	{
		const auto trf = lightView * v;
		minX = std::min(minX, trf.x);
		maxX = std::max(maxX, trf.x);
		minY = std::min(minY, trf.y);
		maxY = std::max(maxY, trf.y);
		minZ = std::min(minZ, trf.z);
		maxZ = std::max(maxZ, trf.z);
	}

	// Tune this parameter according to the scene
	constexpr float zMult = 10.0f;
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

	const Mat4 lightProjection = Mat4::orthographic(Vec3(minX, minY, minZ), Vec3(maxX, maxY, maxZ));

	return lightView * lightProjection;
}

std::vector<Mat4> getLightSpaceMatrices(std::vector<float> cascadeZ, float near, float far, float fov, float aspectRatio, const Mat4& view, Vec3 lightDir)
{
	std::vector<Mat4> ret;
	for (size_t i = 0; i < cascadeZ.size() + 1; ++i)
	{
		if (i == 0)
		{
			ret.push_back(getLightSpaceMatrix(near, cascadeZ[i], fov, aspectRatio, view, lightDir));
		}
		else if (i < cascadeZ.size())
		{
			ret.push_back(getLightSpaceMatrix(cascadeZ[i - 1], cascadeZ[i], fov, aspectRatio, view, lightDir));
		}
		else
		{
			ret.push_back(getLightSpaceMatrix(cascadeZ[i - 1], far, fov, aspectRatio, view, lightDir));
		}
	}
	return ret;
}

/*
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
*/

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

RenderingSystem::RenderingSystem(Scene& scene)
	: m_skyboxData(
		"assets/textures/skybox/right.png",
		"assets/textures/skybox/left.png",
		"assets/textures/skybox/top.png",
		"assets/textures/skybox/bottom.png",
		"assets/textures/skybox/front.png",
		"assets/textures/skybox/back.png")

	, m_chunkShader("src/Game/Rendering/Shaders/Objects/chunk.vert", "src/Game/Rendering/Shaders/Objects/chunk.frag")

	, m_crosshairTexture("assets/textures/crosshair.png")
	, m_squareShader("src/Game/Rendering/Shaders/2dTextured.vert", "src/Game/Rendering/Shaders/2dTextured.frag")
	, m_debugShader("src/Game/Rendering/Shaders/debug.vert", "src/Game/Rendering/Shaders/debug.frag")
	, m_texturedSquareShader("src/Game/Inventory/Shaders/uiTextured.vert", "src/Game/Rendering/Shaders/debugDepth.frag")

	, m_cubeLinesVbo(cubeLinesVertices, sizeof(cubeLinesVertices))
	, m_squareTrianglesVbo(squareTrianglesVertices, sizeof(squareTrianglesVertices))
	, m_pointVbo(point, sizeof(point))
	, m_LineVbo(lineData, sizeof(lineData))
	, m_squareTrianglesVbo2(squareTrianglesVertices2, sizeof(squareTrianglesVertices2))

	, m_geometryBufferFbo(Fbo::generate())
	, m_depthTexture(Texture::generate())
	, m_positionTexture(Texture::generate())
	, m_normalTexture(Texture::generate())
	, m_albedoTexture(Texture::generate())
	, m_defferedPassShader("src/Game/Rendering/Shaders/Deffered/deffered.vert", "src/Game/Rendering/Shaders/Deffered/deffered.frag")

	, m_shadowMapFbo(Fbo::generate())
	, m_cascadeZ({ 10, 30, 50, 60 })
	//, m_cascadeZ({ m_farPlaneZ / 50.0f, m_farPlaneZ / 25.0f, m_farPlaneZ / 10.0f, m_farPlaneZ / 2.0f})
	/*, m_cascadeZ({ m_farPlaneZ / 50.0f, m_farPlaneZ / 25.0f, m_farPlaneZ / 10.0f, m_farPlaneZ / 2.0f })*/
	, ff(Fbo::generate())
	, s(Texture::generate())
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
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}

	m_shadowMapFbo.bind();
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	Fbo::unbind();

	ff.bind();
	s.bind();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, s.handle(), 0);
	GLenum a = GL_COLOR_ATTACHMENT0;
	glDrawBuffers(1, &a);
	Fbo::unbind();
}

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
	// Maybe use GL_DEPTH24_STENCIL8 - better driver compatiblity.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, m_screenSize.x, m_screenSize.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

	// Have to use a floating point texture format for shadows to work.
	for (const auto& texture : m_shadowMapTextures)
	{
		texture.bind();
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, m_screenSize.x, m_screenSize.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, 4096, 4096, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	}

	s.bind();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, m_screenSize.x, m_screenSize.y, 0, GL_RGB, GL_FLOAT, nullptr);

	glViewport(0, 0, m_screenSize.x, m_screenSize.y);
}

void RenderingSystem::update(
	const Vec2& screenSize, 
	const Vec3& cameraPos,
	const Quat& cameraRot, 
	const EntityManager& entites,
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

	static auto f = fov;
	static auto v = Mat4::lookAt(cameraPos, cameraPos + Vec3(1, 2, 3).normalized(), Vec3::up);
	static auto a = aspectRatio;

	m_projection = projection;
	m_view = view;

	m_geometryBufferFbo.bind();
	drawScene(chunkSystem, projection, view, true);
	Renderer::drawSkybox(m_view, m_projection, m_skyboxData);
	drawDebugShapes(projection, view);
	glFrontFace(GL_CW);
	Fbo::unbind();
	
	m_shadowMapFbo.bind();
	//auto ms = getLightSpaceMatrices(fov, aspectRatio, view, Vec3(0.5, -1, 0).normalize(), m_nearPlaneZ, m_farPlaneZ, )
	/*const auto ms = getLightSpaceMatrices(fov, aspectRatio, m_nearPlaneZ, m_farPlaneZ, view, Vec3(-0.5, -1, -0.5).normalized());*/
	//m_cascadeZ = { m_nearPlaneZ, m_farPlaneZ };
	/*auto ms = getLightSpaceMatrices(fov, aspectRatio, view, Vec3(-0.5, -1, -0.5).normalized(), m_nearPlaneZ, m_farPlaneZ, { 10, 30, 50, m_farPlaneZ });*/
	auto ms = getLightSpaceMatrices(fov, aspectRatio, view, Vec3(-0.5, -1, -0.5).normalized(), m_nearPlaneZ, m_farPlaneZ, m_cascadeZ);
	//auto ms = getLightSpaceMatrices(f, a, v, Vec3(-0.5, -1, -0.5).normalized(), m_nearPlaneZ, m_farPlaneZ, m_cascadeZ);
	//auto ms = getLightSpaceMatrices(m_cascadeZ, m_nearPlaneZ, m_farPlaneZ, fov, aspectRatio, view, Vec3(20.0f, 50, 20.0f).normalized());
	/*ms[0] = getLightSpaceMatrix(fov, aspectRatio, m_nearPlaneZ, m_farPlaneZ, view, Vec3(-0.5, -1, -0.5).normalized());*/
	//ms[0] = getLightSpaceMatrix(fov, aspectRatio, m_nearPlaneZ, 10, view, Vec3(-0.5, -1, -0.5).normalized());
	glViewport(0, 0, 4096, 4096);
	for (size_t i = 0; i < m_shadowMapTextures.size(); i++)
	{
		m_shadowMapTextures[i].bind();
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_shadowMapTextures[i].handle(), 0);
		glCullFace(GL_FRONT);
		drawScene(chunkSystem, Mat4::identity, ms[i], false);
		glCullFace(GL_BACK);
	}
	glViewport(0, 0, screenSize.x, screenSize.y);
	Fbo::unbind();

	//ff.bind();
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
	m_defferedPassShader.setMat4("camera", view);
	m_defferedPassShader.setInt("test", 0);
	m_defferedPassShader.setVec3("directionalLightDir", Vec3(-0.5, -1, -0.5).normalized());
	
	m_squareTrianglesVao2.bind();
	glDrawArrays(GL_TRIANGLES, 0, 6);

	//m_defferedPassShader.setInt("test", 1);
	//glClear(GL_DEPTH_BUFFER_BIT);
	//glViewport(0, 0, screenSize.x / 3, screenSize.y / 3);
	//glDrawArrays(GL_TRIANGLES, 0, 6);

	//m_defferedPassShader.setInt("test", 2);
	//glClear(GL_DEPTH_BUFFER_BIT);
	//glViewport(screenSize.x / 3, 0, screenSize.x / 3, screenSize.y / 3);
	//glDrawArrays(GL_TRIANGLES, 0, 6);

	Fbo::unbind();
	glViewport(0, 0, screenSize.x, screenSize.y);


	// Add this line to other rendering.
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void RenderingSystem::drawScene(const ChunkSystem& chunkSystem, const Mat4& projection, const Mat4& view, bool test)
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	if (test)
	{
		glCullFace(GL_BACK);
	}
	else
	{
		//glCullFace(GL_FRONT);
		glCullFace(GL_BACK);
	}

	m_chunkShader.use();
	m_chunkShader.setMat4("projection", projection);
	m_chunkShader.setMat4("camera", view);
	glActiveTexture(GL_TEXTURE0);
	chunkSystem.blockData.textureArray.bind();
	m_chunkShader.setTexture("blockTextureArray", 0);
	chunkSystem.m_vao.bind();
	for (const auto& chunk : chunkSystem.m_chunksToDraw)
	{
		m_chunkShader.setMat4("model", Mat4::translation(Vec3(chunk->pos) * Chunk::SIZE));
		glDrawArrays(GL_TRIANGLES, chunk->vboByteOffset / sizeof(uint32_t), chunk->vertexCount);

		if (Debug::shouldShowChunkBorders)
		{
			Debug::drawCube(Vec3(chunk->pos) * Chunk::SIZE * Block::SIZE + Vec3(Chunk::SIZE) / 2.0, Vec3(Chunk::SIZE), Vec3(1, 1, 1));
		}
	}
	glCullFace(GL_BACK);
}

Mat4 RenderingSystem::getLightSpaceMatrix(float fov, float aspectRatio, const float nearPlane, const float farPlane, const Mat4& view, const Vec3& lightDir)
{
	/*const auto proj = Mat4::perspective(fov, aspectRatio, nearPlane - 40, farPlane + 40);*/
	const auto proj = Mat4::perspective(fov, aspectRatio, nearPlane, farPlane);
	const auto corners = getFrustumCornersWorldSpace(proj, view);

	for (auto c : corners)
	{
		Debug::drawPoint(c);
	}

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
	//min -= Vec3(40);
	//max += Vec3(40);
	auto s = max - min;
	//Debug::drawCube(min + s / 2, s);

	// Tune this parameter according to the scene
	constexpr float zMult = 60.0f;
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

	// Project every 
	const Mat4 lightProjection = Mat4::orthographic(min, max);

	return lightView * lightProjection;
}

std::vector<Mat4> RenderingSystem::getLightSpaceMatrices(float fov, float aspectRatio, const Mat4& view, const Vec3& lightDir, float nearZ, float farZ, const std::vector<float>& shadowCascadeLevels)
{
	std::vector<Mat4> ret;
	for (size_t i = 0; i < shadowCascadeLevels.size() + 1; ++i)
	{
		/*ret.push_back(getLightSpaceMatrix(fov, aspectRatio, nearZ, farZ, view, lightDir));
		continue;*/
		if (i == 0)
		{
			ret.push_back(getLightSpaceMatrix(fov, aspectRatio, nearZ, shadowCascadeLevels[i], view, lightDir));
		}
		else if (i < shadowCascadeLevels.size())
		{
			ret.push_back(getLightSpaceMatrix(fov, aspectRatio, shadowCascadeLevels[i - 1], shadowCascadeLevels[i], view, lightDir));
		}
		else
		{
			ret.push_back(getLightSpaceMatrix(fov, aspectRatio, shadowCascadeLevels[i - 1], farZ, view, lightDir));
		}
	}
	return ret;
	/*std::vector<Mat4> matrices;

	matrices.push_back(getLightSpaceMatrix(fov, aspectRatio, nearZ, shadowCascadeLevels[0], view, lightDir));
	for (size_t i = 1; i < shadowCascadeLevels.size(); i++)
	{
		matrices.push_back(getLightSpaceMatrix(fov, aspectRatio, shadowCascadeLevels[i - 1], shadowCascadeLevels[i], view, lightDir));
	}
	matrices.push_back(getLightSpaceMatrix(fov, aspectRatio, shadowCascadeLevels[shadowCascadeLevels.size() - 1], farZ, view, lightDir));

	return matrices;*/
}

std::array<Vec3, 8> RenderingSystem::getFrustumCornersWorldSpace(const Mat4& proj, const Mat4& view)
{
	// NDC cube corners
	/*std::array<Vec3, 8> corners = {
		Vec3(1,  1,  1),
		Vec3(1, -1,  1),
		Vec3(-1,  1,  1),
		Vec3(-1, -1,  1),
		Vec3(1,  1, -1),
		Vec3(1, -1, -1),
		Vec3(-1,  1, -1),
		Vec3(-1, -1, -1),
	};*/

	std::array<Vec3, 8> corners = {
		Vec3{-1, -1, -1},
		Vec3{-1, - 1,1},
		Vec3{-1,1, - 1},
		Vec3{-1,1,1},
		Vec3{1, - 1, - 1},
		Vec3{1, - 1,1},
		Vec3{1,1, - 1},
		Vec3{1,1,1}
	};

	// To projection matrix projects the frustum into the NDC cube.
	// Doing the inverse will give the frustum coodinates.
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

/*void RenderingSystem::drawChunks(const ChunkSystem& chunkSystem, ShaderProgram& shader)
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

*/
