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
	std::vector<Mat4> nodeTransforms;

	Model::KeyFrame keyframeBeforeTransition;
	float transitionTime;
};