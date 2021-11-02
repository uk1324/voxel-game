#pragma once

#include <Engine/Input/MouseMoveAction.hpp>

class CameraRotateAction : public MouseMoveAction
{
	void execute(double mouseX, double mouseY) override;
};