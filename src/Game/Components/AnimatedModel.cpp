#include <Game/Components/AnimatedModel.hpp>
#include <Engine/Time.hpp>
#include <Math/Interpolation.hpp>

AnimatedModel::AnimatedModel(const Model& model)
	: model(model)
	, startTime(0)
	, currentAnimation(0)
	, currentKeyframe(0)
	, state(AnimatedModelState::NoAnimation)
	, playType(AnimatedModelPlayType::PlayLoop)
{
	nodeTransforms.resize(model.nodes.size(), Mat4::identity);
}

#include <iostream>

static Model::KeyFrame transitionKeyframes(const Model::KeyFrame& a, const Model::KeyFrame& b, float t)
{
	Model::KeyFrame keyframe;
	for (size_t i = 0; i < a.scale.size(); i++)
	{
		keyframe.rotation.push_back(Quat::slerp(a.rotation[i], b.rotation[i], t));
		keyframe.translation.push_back(lerp(a.translation[i], b.translation[i], t));
		keyframe.scale.push_back(lerp(a.scale[i], b.scale[i], t));
	}
	return keyframe;
}

void AnimatedModel::playAnimation(size_t index, AnimatedModelPlayType type, float transitionTime)
{
	// Maybe use ticks so it still works with pausing.
	if (transitionTime > 0)
	{
		const auto elapsed = Time::currentTime() - startTime;
		switch (state)
		{
		case AnimatedModelState::NoAnimation:
			keyframeBeforeTransition = model.animations[currentAnimation][0];
			break;
		case AnimatedModelState::Animation:
		{
			currentKeyframe = calculateCurrentKeyframe(elapsed);
			const auto& keyframes = model.animations[currentAnimation];
			const auto& keyframe = keyframes[currentKeyframe];
			const auto& nextKeyframe = keyframes[currentKeyframe + 1];
			const auto t = (elapsed - keyframe.time) / (nextKeyframe.time - keyframe.time);
			keyframeBeforeTransition = transitionKeyframes(keyframe, nextKeyframe, t);
			break;
		}
		case AnimatedModelState::Transition:
		{
			const auto& keyframe = keyframeBeforeTransition;
			const auto& keyframes = model.animations[index];
			const auto& nextKeyframe = keyframes[0];
			const auto t = elapsed / transitionTime;
			keyframeBeforeTransition = transitionKeyframes(keyframe, nextKeyframe, t);
			break;
		}
		default:
			break;
		}
		state = AnimatedModelState::Transition;
		this->transitionTime = transitionTime;
	}
	else
	{
		state = AnimatedModelState::Animation;
	}
	startTime = Time::currentTime();
	currentKeyframe = 0;
	playType = type;
	currentAnimation = index;
}

static Mat4 interpoleKeyframes(const Model::KeyFrame& a, const Model::KeyFrame& b, size_t nodeIndex, float t)
{
	//ASSERT(t >= 0 && t <= 1);
	const auto scale = lerp(a.scale[nodeIndex], b.scale[nodeIndex], t);
	const auto translation = lerp(a.translation[nodeIndex], b.translation[nodeIndex], t);
	const auto rotation = Quat::slerp(a.rotation[nodeIndex], b.rotation[nodeIndex], t);
	return Mat4::scale(scale) * rotation.asMatrix() * Mat4::translation(translation);
}

static void propagateTransform(const Model& model, std::vector<Mat4>& nodeTransforms, const Model::Node& parent)
{
	for (const auto& child : parent.children)
	{
		nodeTransforms[model.nodeIndex(child)] *= nodeTransforms[model.nodeIndex(&parent)];
		propagateTransform(model, nodeTransforms, *child);
	}
}

void AnimatedModel::update()
{
	auto elapsed = Time::currentTime() - startTime;
	const auto& keyframes = model.animations[currentAnimation];

	if (state == AnimatedModelState::Transition)
	{
		if (elapsed > transitionTime)
		{
			state = AnimatedModelState::Animation;
			currentKeyframe = 0;
			elapsed = fmod(elapsed, transitionTime);
			startTime = Time::currentTime();
		}
	}

	switch (state)
	{
	case AnimatedModelState::NoAnimation:
		for (size_t i = 0; i < model.nodes.size(); i++)
		{
			nodeTransforms[i] = interpoleKeyframes(keyframes[0], keyframes[0], i, 0);
		}
		break;

	case AnimatedModelState::Animation:
		if ((playType == AnimatedModelPlayType::PlayLoop) && (elapsed > keyframes.back().time))
		{
			currentKeyframe = 0;
			elapsed = fmod(elapsed, keyframes.back().time);
			startTime = Time::currentTime();
		}
		currentKeyframe = calculateCurrentKeyframe(elapsed);
		for (size_t i = 0; i < model.nodes.size(); i++)
		{
			const auto& keyframe = keyframes[currentKeyframe];
			const auto& nextKeyframe = keyframes[currentKeyframe + 1];
			const auto t = (elapsed - keyframe.time) / (nextKeyframe.time - keyframe.time);
			nodeTransforms[i] = interpoleKeyframes(keyframe, nextKeyframe, i, t);
		}
		break;

	case AnimatedModelState::Transition:
		for (size_t i = 0; i < model.nodes.size(); i++)
		{
			const auto& keyframe = keyframeBeforeTransition;
			const auto& nextKeyframe = keyframes[0];
			const auto t = elapsed / transitionTime;
			nodeTransforms[i] = interpoleKeyframes(keyframe, nextKeyframe, i, t);
		}
		break;
	}

	nodeTransforms[model.nodeIndex(model.rootNode)] = model.rootNode->defaultPosTransform;
	propagateTransform(model, nodeTransforms, *model.rootNode);
}

size_t AnimatedModel::calculateCurrentKeyframe(float elapsed)
{
	const auto& keyframes = model.animations[currentAnimation];
	for (size_t i = currentKeyframe; i < keyframes.size() - 1; i++)
	{
		if (elapsed < keyframes[i + 1].time)
		{
			return i;
		}
	}
	return 0;
}
