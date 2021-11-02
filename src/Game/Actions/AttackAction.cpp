#include <Game/Actions/AttackAction.hpp>

#include <iostream>

void AttackAction::buttonDown()
{
	std::cout << "Mouse down\n";
}

void AttackAction::buttonHeld()
{
	std::cout << "Mouse held\n";
}

void AttackAction::buttonUp()
{
	std::cout << "Mouse up\n";
}
