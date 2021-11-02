#pragma once

class MouseMoveAction
{
public:
	virtual void execute(double mouseX, double mouseY) = 0;
};