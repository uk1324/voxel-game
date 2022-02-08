#include <Menu/MenuScene.hpp>
#include <Engine/Engine.hpp>
#include <Math/Angles.hpp>


float vertices[] = {
	-1.0f,  1.0f, 0.0f, 0.0f,
	 1.0f, -1.0f, 1.0f, 1.0f,
	-1.0f, -1.0f, 0.0f, 1.0f,
	-1.0f,  1.0f, 0.0f, 0.0f,
	 1.0f,  1.0f, 1.0f, 0.0f,
	 1.0f, -1.0f, 1.0f, 1.0f,
};

MenuScene::MenuScene(Engine& engine)
	: Scene(engine, 1)
	, m_squareTrianglesVbo(vertices, sizeof(vertices))
	, m_fontShader("src/Game/Inventory/Shaders/font.vert", "src/Game/Inventory/Shaders/font.frag")
	, m_buttonShader("src/Menu/button.vert", "src/Menu/button.frag")
	, m_fontTextureArray(32, 32, "assets/textures/ascii_font.png")
	, m_skyboxData(
		"assets/textures/panorama_1.png",
		"assets/textures/panorama_3.png",
		"assets/textures/panorama_4.png",
		"assets/textures/panorama_5.png",
		"assets/textures/panorama_0.png",
		"assets/textures/panorama_2.png")
{
	m_squareTrianglesVbo.bind();
	m_squareTrianglesVao.bind();
	Vao::setAttribute(0, BufferLayout(ShaderDataType::Float, 2, 0, sizeof(float) * 4, false));
	Vao::setAttribute(1, BufferLayout(ShaderDataType::Float, 2, sizeof(float) * 2, sizeof(float) * 4, false));

	input.registerMouseButton("press", MouseButton::Left);

	m_buttons.push_back(Button{ Vec2(0, 0.2), Vec2((0.1 * 5) / 2.0f + 0.1, 0.15), "START" });
	m_buttons.push_back(Button{ Vec2(0, -0.2), Vec2((0.1 * 5) / 2.0f + 0.1, 0.15), "EXIT" });
}

void MenuScene::update()
{
	Vec2 mousePos = pixelScreenPosToUiScreenPos(input.mousePos());

	if (input.isButtonDown("press"))
	{
		if (m_buttons[0].contains(mousePos))
		{
			engine.changeScene(std::make_unique<GameScene>(engine));
			return;
		}
		else if (m_buttons[1].contains(mousePos))
		{
			engine.stop();
			return;
		}
	}

	m_screenSize = Vec2(engine.window().getWindowSize());
	glViewport(0, 0, m_screenSize.x, m_screenSize.y);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	auto projection = Mat4::perspective(degToRad(90.0f), m_screenSize.x / m_screenSize.y, 0.1, 100);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	Renderer::drawSkybox(
		Quat(Time::currentTime() / 2.0f, Vec3::up).asMatrix(),
		Quat(degToRad(45.0f), Vec3::xAxis).asMatrix(),
		m_skyboxData
	);

	glClear(GL_DEPTH_BUFFER_BIT);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);

	m_buttonShader.use();
	m_squareTrianglesVao.bind();
	for (const Button& button : m_buttons)
	{
		m_buttonShader.setVec2("size", button.size);
		m_buttonShader.setVec2("pos", button.pos);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	setupDrawText();
	for (const Button& button : m_buttons)
	{
		drawText(button.text, button.pos + Vec2(0.05 / 2, -0.03 / 2), 0.125);
	}


	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
}

float MenuScene::yPercentToXPercent(float yPercent)
{
	return yPercent / m_screenSize.x * m_screenSize.y;
}

void MenuScene::setupDrawText()
{
	m_squareTrianglesVao.bind();
	glActiveTexture(GL_TEXTURE0);
	m_fontTextureArray.bind();
	m_fontShader.use();
	m_fontShader.setTexture("textureArraySampler", 0);
}

void MenuScene::drawText(std::string_view text, Vec2 pos, float size)
{
	m_fontShader.setVec2("size", Vec2(yPercentToXPercent(size), size));

	Vec2 startPos(pos.x - size * (static_cast<float>(text.length() - 1) / 2.0f), pos.y);

	for (size_t i = 0; i < text.size(); i++)
	{
		m_fontShader.setVec2("pos", startPos + Vec2(size * i, 0.0f));
		m_fontShader.setUnsignedInt("textureIndex", text[i]);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
}

Vec2 MenuScene::pixelScreenPosToUiScreenPos(const Vec2& pos)
{
	Vec2 convertedPos = pos;
	convertedPos /= m_screenSize * 0.5;
	convertedPos -= Vec2(1, 1);
	convertedPos.y = -convertedPos.y;
	return convertedPos;
}

bool MenuScene::Button::contains(const Vec2& p)
{
	return (p.x > pos.x - size.x) && (p.x < pos.x + size.x)
		&& (p.y > pos.y - size.y) && (p.y < pos.y + size.y);
}
