#pragma once
#include <Game.h>
#include <Cube.h>
#include <Pyramid.h>
#include <Timer.h>

class MyGame : public Game
{
public:
	MyGame();
	~MyGame();

	void Update() override;
	void Render() override;

private:
	Pyramid* c = nullptr;
	Timer* m_time;
};

