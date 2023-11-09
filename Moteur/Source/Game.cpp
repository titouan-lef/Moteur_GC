#include "Entity.h"
#include "Engine.h"
#include "SceneManager.h"
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
	delete m_wndManager;
	delete m_InputHandler;
}

int Game::Run()
{
	// Initialize the game
	Initialize();

	// Let's play!
	while (true)
	{
		if (Window::ProcessMessages())
			return 0;

		Update();

		Engine::GetInstance()->Time->Mark();

		m_wndManager->PreRender();
		Render();
		m_wndManager->PostRender();
	}
}

void Game::Update()
{
	SceneManager::GetInstance()->CurrentSceneUpdate();
}

void Game::Render()
{
	SceneManager::GetInstance()->CurrentSceneRender();
}
