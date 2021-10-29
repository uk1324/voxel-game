#pragma once

class Scene;

class System
{
public:
	virtual ~System() = 0;
	virtual void update(Scene& scene) = 0;
};