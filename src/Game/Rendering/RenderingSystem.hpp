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

	void update(float width, float height, const Vec3& cameraPos, const Quat& cameraRot, const EntityManager& entityManger, const ChunkSystem& chunkSystem);

	void drawCube(const Vec3& pos, const Vec3& scale, const Vec3& color);
	void drawPoint(const Vec3& pos, float size, const Vec3& color);

private:
	Gfx::CubeMap m_skyboxTexture;
	Gfx::ShaderProgram m_skyboxShader;

	Gfx::TextureArray m_blockTextureArray;
	Gfx::ShaderProgram m_chunkShader;

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

};

struct Debug
{
public:
	static void drawCube(const Vec3& pos, const Vec3& scale = Vec3(1, 1, 1), const Vec3& color = Vec3(1, 0, 0));
	static void drawPoint(const Vec3& pos, float size = 10.0f, const Vec3& color = Vec3(1, 0, 0));

	static RenderingSystem* renderingSystem;
};