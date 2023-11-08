#pragma once

#include <Scene.h>

class TestSwitchScene : public Scene
{
public:
	TestSwitchScene();
	~TestSwitchScene() {}

	virtual void Init() override;
	virtual void Update() override;

	int test = 0;
};