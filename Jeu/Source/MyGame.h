#pragma once
#include <Game.h>
#include <Cube.h>
#include <Timer.h>
#include "Controller.h"
#include "Player.h"

class MyGame : public Game
{
public:
	MyGame();
	~MyGame();

	virtual void Initialize() override;


private:
	Timer* m_time = nullptr;

	Timer* m_timeR = nullptr;
	std::list<Entity*>* listRock = nullptr;
	Controller* m_controller;
	Player* m_player;
};

