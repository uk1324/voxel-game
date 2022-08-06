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

// Could constructor shaders inside the header file.
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
		Entity playerEntity,
		const Vec3& cameraPos, 
		const Quat& cameraRot, 
		EntityManager& entityManger, 
		const ChunkSystem& chunkSystem, 
		const ItemData& itemData,
		const InputManager& input,
		const Opt<ItemStack>& heldItem);

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

	void drawScene(
		EntityManager& entityManager,
		const ItemData& itemData,
		const ChunkSystem& chunkSystem,
		const Mat4& projection,
		const Mat4& view);
	void resetStatistics();

	std::pair<Mat4, Vec3> getShadowMatrix(float fov, float aspectRatio, const float nearPlane, const float farPlane, const Mat4& proj, const Mat4& view, const Vec3& lightDir);
	std::vector<std::pair<Mat4, Vec3>> getShadowMatrices(float fov, float aspectRatio, const Mat4& proj, const Mat4& view, const Vec3& lightDir, float nearZ, float farZ, const std::vector<float>& shadowCascadeLevels);

	void itemBlockShaderSetPerPass(const Mat4& view, const Mat4& projection);
	void itemBlockDraw(const BlockData::Entry& blockInfo, const Mat4& model);

	void waterShaderConfig();
	void drawTexturedQuad(Texture& texture);

private:
	static constexpr float m_farPlaneZ = 20.0 * Chunk::SIZE_X;
	float m_nearPlaneZ = 0.1f;
	float m_fov = degToRad(90.0f);
	float zMult = 160.0f;

	static constexpr size_t SHADOW_MAP_SIZE = 2048;

	Vec3 m_directionalLightDir = Vec3(-0.5, -1, -0.5).normalized();

	Fbo m_debugFbo;
	Texture m_debugTexture;
	
	Fbo m_geometryBufferFbo;
	Texture m_depthTexture, m_positionTexture, m_normalTexture, m_albedoTexture;

	ShaderProgram m_chunkShader;
	ShaderProgram m_chunkWaterShader;
	ShaderProgram m_itemBlockShader;
	ShaderProgram m_itemModelShader;
	ShaderProgram m_modelShader;

	ShaderProgram m_blockParticleShader;
	Vbo m_blockParticleVerticesVbo;
	Vbo m_blockParticleVbo;
	Vao m_blockParticleVao;
	struct BlockParticleInstance
	{
		Mat4 model;
		uint32_t blockTextureIndex;
	};
	static constexpr size_t MAX_PARTICLES_PER_INSTANCED_DRAW_CALL = 100;
	std::array<BlockParticleInstance, MAX_PARTICLES_PER_INSTANCED_DRAW_CALL> m_blockParticleInstances;


	Fbo m_shadowMapFbo;
	// Make shadowMapTextures store depth and color so stained glass casts colored shadows.
	std::vector<Texture> m_shadowMapTextures;
	std::vector<float> m_cascadeZ;

	ShaderProgram m_defferedPassShader;
	ShaderProgram m_defferedDrawNormals;
	ShaderProgram m_defferedDrawAlbedo;
	ShaderProgram m_defferedDrawDepth;

	Fbo m_postProcessFbo0;
	Texture m_postprocessTexture0;
	Texture m_postprocessDepthTexture0;

	Fbo m_postProcessFbo1;
	Texture m_postprocessTexture1;
	Texture m_postprocessDepthTexture1;

	ShaderProgram m_postProcessShader;
	ShaderProgram m_postProcessWaterShader;

	Vec2 m_screenSize;

	SkyboxData m_skyboxData;

	Vao m_squareTrianglesVao2;
	Vbo m_squareTrianglesVbo2;

	ShaderProgram m_squareShader;
	Texture m_crosshairTexture;

	ShaderProgram m_texturedQuad;

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

	int m_selectedDefferedDrawItem;
	bool m_enableShadowMapPass;
	size_t m_totalChunksToDraw = 0;
	size_t m_chunksDrawn = 0;
	size_t m_totalBlockItemsToDraw = 0;
	size_t m_blockItemsDrawn = 0;
	size_t m_totalVoxelizedTextureItemsToDraw = 0;
	size_t m_voxelizedTextureItemsDrawn = 0;
	size_t m_totalMeshesToDraw = 0;
	size_t m_meshesDrawn = 0;

	int octaves = 1;
	float persistence = 0.5;
	float diagonalNormalOffset = 1;
	float inputNoiseScale = 0.5;
	int specularIntensity = 10;
	float diagonalScale = 1;
	float diagonalNoiseScale = 0.2;
	Vec3 colorWater = Vec3(13 / 256.0, 53 / 256.0, 82 / 256.0);
	Vec3 colorSpecular = Vec3(55 / 255.0f, 62 / 255.0f, 72 / 255.0f);
	float timeScale = 1;
	Vec3 color;
};