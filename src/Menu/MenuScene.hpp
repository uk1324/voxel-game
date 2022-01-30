#pragma once

#include <Engine/Scene.hpp>
#include <Engine/Graphics/ShaderProgram.hpp>
#include <Engine/Graphics/Vao.hpp>
#include <Engine/Graphics/TextureArray.hpp>
#include <Game/GameScene.hpp>
#include <Engine/Renderer/Renderer.hpp>

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

	ShaderProgram m_buttonShader;

	ShaderProgram m_fontShader;
	TextureArray m_fontTextureArray;

	Vao m_squareTrianglesVao;
	Vbo m_squareTrianglesVbo;

	SkyboxData m_skyboxData;
};