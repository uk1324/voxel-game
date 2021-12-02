#pragma once

#include <stdint.h>

class Time
{
public:
	Time();

	float deltaTime();
	uint64_t currentTick();

	static double timestamp();

	void update();

private:
	double m_deltaTime;
	double m_lastFrameTime;	

	uint64_t m_currentTick;
};