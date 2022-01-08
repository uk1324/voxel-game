#pragma once

struct Color
{
public:
	Color(float r, float g, float b, float a);

	float* data();
	const float* data() const;

public:
	float r, g, b, a;
};