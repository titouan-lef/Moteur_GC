
#include "Entity.h"
#include "Game.h"

Game::Game(const wchar_t* name, UINT width, UINT height, UINT x, UINT y) : m_window(Window(name, width, height, x, y))
{
	m_wndManager = new WindowManager(width, height, m_window.GethWnd());
}

Game::Game(const wchar_t* name, UINT width, UINT height) : m_window(Window(name, width, height))
{
	m_wndManager = new WindowManager(width, height, m_window.GethWnd());
}

Game::Game() : m_window(Window(L"Game", 800, 600))
{
	m_wndManager = new WindowManager(800, 600, m_window.GethWnd());
}

Game::~Game()
{
}

int Game::Run()
{
	while (true)
	{
		if (Window::ProcessMessages())
			return 0;

		Update();

		m_wndManager->PreRender();
		Render();
		m_wndManager->PostRender();
	}
}
