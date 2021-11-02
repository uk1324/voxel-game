#include "JumpAction.hpp"

#include <iostream>

void JumpAction::buttonDown()
{
	std::cout << "Jump down\n";
}

void JumpAction::buttonHeld()
{
	std::cout << "Jump held\n";
}

void JumpAction::buttonUp()
{
	std::cout << "Jump up\n";
}
