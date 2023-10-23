#pragma once
#include "MyWindow.h"
#include "Timer.h"
class App
{
public:
	App();
	//boucle de message
	int Go();
private:
	void DoFrame();
private:
	MyWindow wnd;
	Timer timer;
};

