#pragma once

template<typename T>
static constexpr T pi = 3.14159265359;

template<typename T>
constexpr T degToRad(T angle);
template<typename T>
constexpr T radToDeg(T angle);

template<typename T>
constexpr T degToRad(T deg)
{
	return deg * (pi<T> / 180);
}

template<typename T>
constexpr T radToDeg(T deg)
{
	return deg * (180 / pi<T>);
}
