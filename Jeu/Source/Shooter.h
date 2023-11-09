#pragma once

#include <Timer.h>
#include "Controller.h"
#include <Component.h>

class Timer;

class Shooter : public Component
{
public:
	Shooter();
	~Shooter();

	virtual void Initialize() override;
	virtual void Update() override;

	void SetController(Controller* c) { this->controller = c; }
private:
	Timer *timer = nullptr;
	Controller *controller = nullptr;
	float cooldown = 0.5f;
};