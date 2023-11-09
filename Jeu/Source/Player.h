#pragma once
#include <Timer.h>
#include "Controller.h"
#include <Entity.h>
class Player : public Entity
{
public:
	Player();
	~Player();
	void Init();
	void Update();
	void IsHit(std::list<Entity*>* listRock);
	void IsDead();

private:
	Timer* m_time;
	Controller* m_controller;
	float m_lifePoint;
	bool m_isDead;

};

