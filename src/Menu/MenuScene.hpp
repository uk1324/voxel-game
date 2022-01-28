#pragma once

#include <Engine/Scene.hpp>
#include <Engine/Graphics/ShaderProgram.hpp>
#include <Engine/Graphics/VertexArray.hpp>
#include <Engine/Graphics/TextureArray.hpp>
#include <Game/GameScene.hpp>

class MenuScene : public Scene
{
private:
	class Button
	{
	public:
		Vec2 pos;
		Vec2 size;
		std::string text;
		float fontSize;

	public:
		bool contains(const Vec2& pos);
	};

public:
	MenuScene(Engine& engine);

	void update() override;

	float yPercentToXPercent(float yPercent);

	void setupDrawText();
	void drawText(std::string_view text, Vec2 pos, float size);

	Vec2 pixelScreenPosToUiScreenPos(const Vec2& pos);

private:

	std::vector<Button> m_buttons;

	Vec2 m_screenSize;

	Gfx::ShaderProgram m_buttonShader;

	Gfx::ShaderProgram m_fontShader;
	Gfx::TextureArray m_fontTextureArray;

	Gfx::VertexArray m_squareTrianglesVao;
	Gfx::VertexBuffer m_squareTrianglesVbo;

	Gfx::VertexArray m_cubeTrianglesVao;
	Gfx::VertexBuffer m_cubeTrianglesVbo;

	Gfx::CubeMap m_skyboxTexture;
	Gfx::ShaderProgram m_skyboxShader;
};