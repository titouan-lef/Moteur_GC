#pragma once
#include "Window.h"
#include "WindowManager.h"

class Entity;

class Game
{
public:
	Game(const wchar_t* name, UINT width, UINT height, UINT x, UINT y);
	Game(const wchar_t* name, UINT width, UINT height);
	Game();
	~Game();

	int Run();

	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;

	void Render(Entity e);

private:
	Window m_window;
	WindowManager* m_wndManager = nullptr;
};

