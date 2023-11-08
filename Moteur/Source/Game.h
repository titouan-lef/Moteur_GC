#pragma once
#include "Window.h"
#include "WindowManager.h"
#include "../../Jeu/Source/Controller.h"

class Entity;

class Game
{
public:
	Game(const wchar_t* name, UINT width, UINT height, UINT x, UINT y);
	Game(const wchar_t* name, UINT width, UINT height);
	Game();
	~Game();

	int Run();

	virtual void Initialize() = 0; // Called once before the game loop, put your scene here
	void Update();
	void Render();
protected:
	Controller* m_InputHandler = nullptr;
private:
	Window m_window;
	WindowManager* m_wndManager = nullptr;
};

