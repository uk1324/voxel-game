#include <Game/Debug/Debug.hpp>
#include <Engine/Input/InputManager.hpp>

void Debug::init(InputManager& input, RenderingSystem& renderingSystem)
{
	s_renderingSystem = &renderingSystem;
	input.registerKeyboardButton("debug", KeyCode::F3);
	input.registerKeyboardButton("debugShowChunkBorders", KeyCode::G);
	input.registerKeyboardButton("shouldShowHitboxes", KeyCode::B);
	input.registerKeyboardButton("shouldDisplayRendererSettings", KeyCode::R);
}

void Debug::update(const InputManager& input)
{
	if (input.isButtonHeld("debug"))
	{
		if (input.isButtonDown("debugShowChunkBorders"))
		{
			shouldShowChunkBorders = !shouldShowChunkBorders;
		}

		if (input.isButtonDown("shouldShowHitboxes"))
		{
			shouldShowHitboxes = !shouldShowHitboxes;
		}

		if (input.isButtonDown("shouldDisplayRendererSettings"))
		{
			shouldDisplayRendererSettings = !shouldDisplayRendererSettings;
		}
	}
}

void Debug::drawCube(const Vec3& pos, const Vec3& scale, const Vec3& color)
{
	s_renderingSystem->drawCube(pos, scale, color);
}

void Debug::drawPoint(const Vec3& pos, float size, const Vec3& color)
{
	s_renderingSystem->drawPoint(pos, size, color);
}

void Debug::drawLine(const Vec3& startPos, const Vec3& endPos, const Vec3& color)
{
	s_renderingSystem->drawLine(startPos, endPos, color);
}

RenderingSystem* Debug::s_renderingSystem = nullptr;

bool Debug::shouldShowChunkBorders = false;
bool Debug::shouldShowHitboxes = false;
bool Debug::shouldDisplayRendererSettings = false;