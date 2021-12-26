#pragma once

template<typename T>
static constexpr T PI = T(3.14159265359);

template<typename T>
constexpr T degToRad(T angle);
template<typename T>
constexpr T radToDeg(T angle);

template<typename T>
constexpr T degToRad(T deg)
{
	return deg * (PI<T> / T(180));
}

template<typename T>
constexpr T radToDeg(T deg)
{
	return deg * (T(180) / PI<T>);
}
