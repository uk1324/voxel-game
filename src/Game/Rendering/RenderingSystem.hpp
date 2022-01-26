#pragma once

#include <Engine/Scene.hpp>
#include <Engine/Ecs/EntityManager.hpp>
#include <Engine/Graphics/CubeMap.hpp>
#include <Engine/Graphics/TextureArray.hpp>
#include <Engine/Graphics/Texture.hpp>
#include <Engine/Graphics/ShaderProgram.hpp>
#include <Game/Blocks/ChunkSystem.hpp>
#include <Math/Quat.hpp>

// If I later need to reuse this I put the main functionality into functions and inherit from it.
// Some members could be static
// Make a global debug class that will store the instance of this class.

class RenderingSystem
{
public:
	RenderingSystem(Scene& scene);

	void update(const Vec2& screenSize, const Vec3& cameraPos, const Quat& cameraRot, const EntityManager& entityManger, const ChunkSystem& chunkSystem);

	void drawCube(const Vec3& pos, const Vec3& scale, const Vec3& color);
	void drawPoint(const Vec3& pos, float size, const Vec3& color);
	void drawLine(const Vec3& startPos, const Vec3& endPos, const Vec3& color);

private:
	static void drawChunks(const ChunkSystem& chunkSystem, Gfx::ShaderProgram& shader);

	void drawSkybox(const Mat4& projection, const Mat4& view);

	// Later use a UBO;
	void drawDebugShapes(const Mat4& projection, const Mat4& view);

	void drawCrosshair(const Vec2& screenSize);

private:
	static std::vector<Vec3> getFrustumCornersWorldSpace(const Mat4& proj, const Mat4& view);
	static Mat4 getLightSpaceMatrix(float fov, float nearZ, float farZ, float aspectRatio, const Mat4& view);
	std::vector<Mat4> RenderingSystem::getLightSpaceMatrices(float fov, float aspectRatio, const Vec3& pos, const Quat& rot);

private:
	Gfx::CubeMap m_skyboxTexture;
	Gfx::ShaderProgram m_skyboxShader;

	const unsigned int SHADOW_WIDTH = 4096 / 2, SHADOW_HEIGHT = 4096 / 2;

	/*unsigned int depthMapFBO;
	unsigned int depthMap;*/
	uint32_t lightFBO;
	uint32_t lightDepthMaps;
	static constexpr unsigned int depthMapResolution = 4096;
	std::vector<float> shadowCascadeLevels;
	static constexpr float farZ = 5 * 16;
	static constexpr float nearZ = 0.1;

	float a1;
	float a2;
	float a3;
	float a4;
	float a5;

	int index = 0;
	unsigned int matricesUBO;

	Gfx::ShaderProgram m_chunkShader;
	Gfx::ShaderProgram m_chunkShadowShader;

	Gfx::ShaderProgram m_texturedSquareShader;
	Gfx::VertexArray m_squareTrianglesVao2;
	Gfx::VertexBuffer m_squareTrianglesVbo2;

	Gfx::ShaderProgram m_squareShader;
	Gfx::Texture m_crosshairTexture;

	Gfx::ShaderProgram m_debugShader;

	Gfx::VertexArray m_cubeTrianglesVao;
	Gfx::VertexBuffer m_cubeTrianglesVbo;

	Gfx::VertexArray m_cubeLinesVao;
	Gfx::VertexBuffer m_cubeLinesVbo;

	Gfx::VertexArray m_squareTrianglesVao;
	Gfx::VertexBuffer m_squareTrianglesVbo;

	Gfx::VertexArray m_pointVao;
	Gfx::VertexBuffer m_pointVbo;

	Gfx::VertexArray m_LineVao;
	Gfx::VertexBuffer m_LineVbo;

	struct Cube
	{
		Vec3 pos;
		Vec3 scale;
		Vec3 color;
	};

	// Have to store them because they would get cleared if I draw them asynchronously.
	std::vector<Cube> m_cubesToDraw;

	struct Point
	{
		Vec3 pos;
		float size;
		Vec3 color;
	};

	std::vector<Point> m_pointsToDraw;

	struct Line
	{
		Vec3 startPos;
		Vec3 scale;
		Vec3 color;
	};

	std::vector<Line> m_linesToDraw;
};