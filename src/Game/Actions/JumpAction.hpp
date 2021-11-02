#pragma once

#include <Engine/Input/ButtonAction.hpp>

class JumpAction : public ButtonAction
{
	void buttonDown() override;
	void buttonHeld() override;
	void buttonUp() override;
};