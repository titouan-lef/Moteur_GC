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
	void IsDead();

	void OnCollision(Entity* e);
private:
	Timer* m_time = nullptr;
	Controller* m_controller = nullptr;
	bool m_isDead;
};