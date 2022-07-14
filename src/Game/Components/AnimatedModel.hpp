#pragma once

#include <Model/Model.hpp>

enum class AnimatedModelState
{
	NoAnimation,
	Animation,
	Transition,
};

enum class AnimatedModelPlayType
{
	PlayLoop,
	PlayClamp
};

struct AnimatedModel
{
	AnimatedModel(const Model& model);

	void playAnimation(size_t index, AnimatedModelPlayType type, float transitionTime);
	void update();
private:
	size_t calculateCurrentKeyframe(float elapsed);

public:
	size_t currentAnimation;
	size_t currentKeyframe;
	float startTime;
	const Model& model;
	AnimatedModelState state;
	AnimatedModelPlayType playType;

	// There is no reason to store those inside each animator. There could be array of transforms in the renderer that would be reused
	// across entites.
	std::vector<Mat4> nodeTransforms;

	// Not sure what is a better way to do this. How to handle cases when blending of an animation is interrupted and then that blending
	// is also interrupted and so on. Could finish the interpolating to one animation and only then start the new one, but that would probably
	// look weird.
	Model::KeyFrame keyframeBeforeTransition;
	float transitionTime;
};