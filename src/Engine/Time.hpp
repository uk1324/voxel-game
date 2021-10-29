#pragma once

#include <stdint.h>

class Time
{
public:
	Time();

	double deltaTime();
	uint64_t currentTick();

	void update();

private:
	double m_deltaTime;
	double m_lastFrameTime;	

	uint64_t m_currentTick;
};