#include <Game/Player/PlayerAnimationComponent.hpp>
#include <Engine/Time.hpp>
#include <algorithm>

void PlayerAnimationComponent::playSwingAnimation()
{
	float progress = swingProgress();
	if (progress >= 1.0f)
	{
		swingStartTime = Time::currentTime();
	}
}

float PlayerAnimationComponent::swingProgress() const
{
	return std::clamp((Time::currentTime() - swingStartTime) / SWING_DURATION, 0.0f, 1.0f);
}
