#pragma once
#include "Window.h"
#include "Timer.h"
#include <vector>
#include <memory>
class App
{
public:
	App();
	//boucle de message
	int Go();

private:
	Window wnd;

};

