#pragma once

struct PlayerMovementComponent
{
public:
	PlayerMovementComponent();

public:
	// I wanted to implement the camera using only quaternions but I couldn't find a good way to constrain a quaternion in a range.
	float rotationX;
	float rotationY;
};