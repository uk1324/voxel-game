#pragma once

#include <Engine/Scene.hpp>
#include <Engine/Ecs/EntityManager.hpp>
#include <Engine/Graphics/TextureArray.hpp>
#include <Engine/Graphics/Texture.hpp>
#include <Engine/Graphics/ShaderProgram.hpp>
#include <Engine/Graphics/Fbo.hpp>
#include <Game/Blocks/ChunkSystem.hpp>
#include <Game/Item/ItemData.hpp>
#include <Math/Quat.hpp>
#include <Engine/Renderer/Renderer.hpp>
#include <Math/Angles.hpp>
#include <Model/Model.hpp>
#include <Math/Frustum.hpp>

// If I later need to reuse this I put the main functionality into functions and inherit from it.
// Some members could be static
// Make a global debug class that will store the instance of this class.

class RenderingSystem
{
private:
	struct Cube
	{
		Vec3 pos;
		Vec3 scale;
		Vec3 color;
	};

	struct Point
	{
		Vec3 pos;
		float size;
		Vec3 color;
	};

	struct Line
	{
		Vec3 startPos;
		Vec3 scale;
		Vec3 color;
	};

public:
	RenderingSystem(Scene& scene);

	void update(
		const Vec2& screenSize, 
		const Vec3& cameraPos, 
		const Quat& cameraRot, 
		EntityManager& entityManger, 
		const ChunkSystem& chunkSystem, 
		const ItemData& itemData);
private:
	void onScreenResize();

public:
	void drawCube(const Vec3& pos, const Vec3& scale, const Vec3& color);
	void drawPoint(const Vec3& pos, float size, const Vec3& color);
	void drawLine(const Vec3& startPos, const Vec3& endPos, const Vec3& color);

private:

	void drawChunks(const ChunkSystem& chunkSystem, ShaderProgram& shader);
	void drawDebugShapes(const Mat4& projection, const Mat4& view);
	void drawCrosshair(const Vec2& screenSize);

	void shadowMapSetup();
	void drawToShadowMap(const ChunkSystem& chunkSystem, const EntityManager& entityManager, const ItemData& itemData);
	void drawScene(
		EntityManager& entityManager,
		const ItemData& itemData,
		const ChunkSystem& chunkSystem,
		const Mat4& projection,
		const Mat4& view);

	std::array<Vec3, 8> getFrustumCornersWorldSpace(const Mat4& proj, const Mat4& view);
	Mat4 getLightSpaceMatrix(float fov, float aspectRatio, const float nearPlane, const float farPlane, const Mat4& view, const Vec3& lightDir);
	std::vector<Mat4> RenderingSystem::getLightSpaceMatrices(float fov, float aspectRatio, const Mat4& view, const Vec3& lightDir, float nearZ, float farZ, const std::vector<float>& shadowCascadeLevels);

private:
	float m_farPlaneZ = 20.0 * Chunk::SIZE_X;
	float m_nearPlaneZ = 0.1f;
	float m_fov = degToRad(90.0f);

	static constexpr size_t SHADOW_MAP_SIZE = 2048;

	float a = 0.0;
	float b = 0.0;

	Vec3 m_directionalLightDir;

	size_t currentKeyframeIndex = 0;
	float startTime;

	float elapsed = 0;

	Fbo m_debugFbo;
	Texture m_debugTexture;
	
	Fbo m_geometryBufferFbo;
	Texture m_depthTexture, m_positionTexture, m_normalTexture, m_albedoTexture;

	ShaderProgram m_chunkShader;
	ShaderProgram m_itemBlockShader;
	ShaderProgram m_itemModelShader;
	ShaderProgram m_modelShader;

	Fbo m_shadowMapFbo;
	std::vector<Texture> m_shadowMapTextures;
	std::vector<float> m_cascadeZ;

	ShaderProgram m_defferedPassShader;

	Vec2 m_screenSize;

	SkyboxData m_skyboxData;

	ShaderProgram m_texturedSquareShader;
	Vao m_squareTrianglesVao2;
	Vbo m_squareTrianglesVbo2;

	ShaderProgram m_squareShader;
	Texture m_crosshairTexture;

	ShaderProgram m_debugShader;

	Vao m_cubeTrianglesVao;
	Vbo m_cubeTrianglesVbo;

	Vao m_cubeLinesVao;
	Vbo m_cubeLinesVbo;

	Vao m_squareTrianglesVao;
	Vbo m_squareTrianglesVbo;

	Vao m_pointVao;
	Vbo m_pointVbo;

	Vao m_LineVao;
	Vbo m_LineVbo;

	// Have to store them because they would get cleared if I drew them asynchronously.
	std::vector<Cube> m_cubesToDraw;
	std::vector<Point> m_pointsToDraw;
	std::vector<Line> m_linesToDraw;
};