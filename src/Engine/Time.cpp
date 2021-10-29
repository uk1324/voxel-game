#include "Time.hpp"

#include <GLFW/glfw3.h>

Time::Time()
	: m_deltaTime(0.0)
	, m_lastFrameTime(0.0)
	, m_currentTick(0)
{}

double Time::deltaTime()
{
	return m_deltaTime;
}

uint64_t Time::currentTick()
{
	return m_currentTick;
}

void Time::update()
{
	double currentTime = glfwGetTime();
	m_deltaTime = currentTime - m_lastFrameTime;
	m_lastFrameTime = currentTime;

	m_currentTick++;
}