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

// If I later need to reuse this I put the main functionality into functions and inherit from it.
// Some members could be static
// Make a global debug class that will store the instance of this class.

struct ModelComponent
{

};

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

	void update(const Vec2& screenSize, const Vec3& cameraPos, const Quat& cameraRot, const EntityManager& entityManger, const ChunkSystem& chunkSystem, const ItemData& itemData);

	void drawCube(const Vec3& pos, const Vec3& scale, const Vec3& color);
	void drawPoint(const Vec3& pos, float size, const Vec3& color);
	void drawLine(const Vec3& startPos, const Vec3& endPos, const Vec3& color);

private:
	void drawChunks(const ChunkSystem& chunkSystem, ShaderProgram& shader);
	void drawDebugShapes(const Mat4& projection, const Mat4& view);
	void drawCrosshair(const Vec2& screenSize);

	void shadowMapSetup();
	void drawToShadowMap(const ChunkSystem& chunkSystem, const EntityManager& entityManager, const ItemData& itemData);
	void drawScene(const ChunkSystem& chunkSystem, const EntityManager& entityManager, const ItemData& itemData);

	void onScreenResize();

	std::array<Vec3, 8> getFrustumCornersWorldSpace(const Mat4& proj, const Mat4& view);
	Mat4 getLightSpaceMatrix(float fov, float aspectRatio, const float nearPlane, const float farPlane, const Mat4& view, const Vec3& lightDir);
	std::vector<Mat4> RenderingSystem::getLightSpaceMatrices(float fov, float aspectRatio, const Mat4& view, const Vec3& lightDir, float nearZ, float farZ, const std::vector<float>& shadowCascadeLevels);

private:
	static constexpr size_t DEPTH_MAP_RESOLUTION = 2048;

private:

	size_t keyframe = 0;

	ShaderProgram m_itemModelShader;
	ShaderProgram m_itemBlockShader;

	Vao m_cubeTrianglesVao;
	Vbo m_cubeTrianglesVbo;

	Fbo m_fbo;
	Texture m_fboTexture;
	Texture m_fboDepthTexture;
	ShaderProgram m_fboShader;

	ShaderProgram m_modelShader;

	Model m_model;

	float m_farPlaneZ = 6.0 * 16.0f;
	float m_nearPlaneZ = 0.1f;
	float m_fov = degToRad(90.0f);
	Vec3 m_directionalLightDir = Vec3(20.0f, 50, 20.0f).normalized();
	Vec2 m_screenSize;

	Mat4 m_projection;
	Mat4 m_view;

	Fbo m_shadowMapFbo;
	TextureArray m_shadowMapTextures;
	unsigned int matricesUBO;

	std::vector<float> shadowCascadeLevels{ m_farPlaneZ / 50.0f, m_farPlaneZ / 25.0f, m_farPlaneZ / 10.0f, m_farPlaneZ / 2.0f };

	SkyboxData m_skyboxData;

	ShaderProgram m_chunkShader;
	ShaderProgram m_chunkShadowShader;

	ShaderProgram m_texturedSquareShader;
	Vao m_squareTrianglesVao2;
	Vbo m_squareTrianglesVbo2;

	ShaderProgram m_squareShader;
	Texture m_crosshairTexture;

	ShaderProgram m_debugShader;

	Vao m_cubeLinesVao;
	Vbo m_cubeLinesVbo;

	Vao m_squareTrianglesVao;
	Vbo m_squareTrianglesVbo;

	Vao m_pointVao;
	Vbo m_pointVbo;

	Vao m_LineVao;
	Vbo m_LineVbo;

	// Have to store them because they would get cleared if I draw them asynchronously.
	std::vector<Cube> m_cubesToDraw;
	std::vector<Point> m_pointsToDraw;
	std::vector<Line> m_linesToDraw;
};