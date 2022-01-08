#include <Math/Color.hpp>

Color::Color(float r, float g, float b, float a)
	: r(r)
	, g(g)
	, b(b)
	, a(a)
{}

float* Color::data()
{
	return reinterpret_cast<float*>(this);
}

const float* Color::data() const
{
	return reinterpret_cast<const float*>(this);
}
