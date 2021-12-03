#include "Time.hpp"

#include <GLFW/glfw3.h>

Time::Time()
	: m_deltaTime(0.0)
	, m_lastFrameTime(0.0)
	, m_currentTick(0)
{}

float Time::deltaTime()
{
	return static_cast<float>(m_deltaTime);
}

uint64_t Time::currentTick()
{
	return m_currentTick;
}

double Time::currentTime()
{
	return glfwGetTime();
}

void Time::update()
{
	// Don't know if I should use the actual delta time or should it be a constant so every program execution is the same.
	double currentTime = glfwGetTime();
	m_deltaTime = currentTime - m_lastFrameTime;
	m_lastFrameTime = currentTime;

	m_currentTick++;
}