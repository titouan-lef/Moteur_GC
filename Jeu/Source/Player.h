#pragma once
#include <Timer.h>
#include "Controller.h"
#include <Entity.h>
class Player : public Entity
{
public:
	Player();
	~Player();

	virtual void Init() override;
	virtual void Update() override;

	void OnCollision(Entity* e);
private:
	Timer* m_time;
	Controller* m_controller;
};

