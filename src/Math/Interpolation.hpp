#pragma once

template<typename T, typename U>
T lerp(T a, T b, U t)
{
	// This is technically better than a + (b - a) * t because it isn't as affected by rounding errors.
	return a * (U(1) - t) + b * t;
}