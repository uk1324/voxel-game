#pragma once

#include <Game/Rendering/RenderingSystem.hpp>

struct Debug
{
public:
	static void init(InputManager& input, RenderingSystem& renderingSystem);
	static void update(const InputManager& input);

	static void drawCube(const Vec3& pos, const Vec3& scale = Vec3(1), const Vec3& color = Vec3(1, 0, 0));
	static void drawPoint(const Vec3& pos, float size = 10.0f, const Vec3& color = Vec3(1, 0, 0));
	static void drawLine(const Vec3& startPos, const Vec3& endPos, const Vec3& color = Vec3(1, 0, 0));

	static bool shouldShowChunkBorders;
	static bool shouldShowHitboxes;
	static bool shouldDisplayRendererSettings;

private:
	static RenderingSystem* s_renderingSystem;
};