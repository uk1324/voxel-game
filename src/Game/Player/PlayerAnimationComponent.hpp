#pragma once

struct PlayerAnimationComponent
{
	void playSwingAnimation();
	float swingProgress() const;

	float swingStartTime = 0.0f;

	static constexpr float SWING_DURATION = 0.3f;
};