#pragma once

// Later split inventory renering and data

#include <Engine/Graphics/VertexArray.hpp>
#include <Engine/Graphics/ShaderProgram.hpp>
#include <Engine/Graphics/Texture.hpp>
#include <Engine/Input/InputManager.hpp>
#include <Game/Blocks/BlockData.hpp>

class Inventory
{
public:
	Inventory();

	void render(const BlockData& blockData, const InputManager& input, float width, float height);

private:
	Gfx::VertexArray m_squareVao;
	Gfx::VertexBuffer m_squareVbo;
	Gfx::VertexArray m_cubeTrianglesVao;
	Gfx::VertexBuffer m_cubeTrianglesVbo;

	Gfx::ShaderProgram m_uiShader;
	Gfx::ShaderProgram m_3dItemShader;

	Gfx::Texture m_inventoryCellTexture;
};