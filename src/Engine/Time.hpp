#pragma once

#include <stdint.h>

class Time
{
public:
	Time();

	float deltaTime() const;
	uint64_t currentTick() const;

	static double currentTime();

	void update();

private:
	double m_deltaTime;
	double m_lastFrameTime;	

	uint64_t m_currentTick;
};