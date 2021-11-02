#pragma once

class ButtonAction
{
public:
	virtual ~ButtonAction() {};

	virtual void buttonDown() = 0;
	virtual void buttonHeld() = 0;
	virtual void buttonUp() = 0;

};