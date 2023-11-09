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


private:
	Timer* m_time;
	Controller* m_controller;


};

