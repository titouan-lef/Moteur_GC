#pragma once
#include <Game.h>
#include <Cube.h>

class MyGame : public Game
{
public:
	MyGame();
	~MyGame();

	void Update() override;
	void Render() override;

private:
	Cube* c = nullptr;
};

