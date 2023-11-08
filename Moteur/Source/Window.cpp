#include "WindowManager.h"
#include "Window.h"
#include "Engine.h"

/*
* _x et _y : coordonées (x, y) du coin supérieur gauche de la fenêtre
*/
Window::Window(const wchar_t* name, UINT width, UINT height, UINT x, UINT y)
	:
	m_name(name),
	m_width(width),
	m_height(height),
	m_hInstance(GetModuleHandle(nullptr))// GetModuleHandle(nullptr)  : handle vers notre application
{
	Engine::SaveWindowSize(width, height);
	// Création d'une classe de fenêtre
	WNDCLASS wc = {};
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = m_hInstance;
	wc.lpszClassName = m_windowName;
	RegisterClass(&wc);

	// Agrandissement de la taille de la fenêtre pour prendre en compte son style et ses bordures
	RECT winRect{};
	winRect.left = x;
	winRect.right = x + width;
	winRect.top = y;
	winRect.bottom = y + height;
	AdjustWindowRect(&winRect, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);// (voir readme Windows.h)

	// Création de la fenêtre (voir readme Windows.h)
	m_hWnd = CreateWindow(
		m_windowName, m_name,
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		x, y, winRect.right - winRect.left, winRect.bottom - winRect.top,
		nullptr, nullptr, m_hInstance, this
	);// (voir readme Windows.h)

	// Affichage de la fenêtre
	ShowWindow(m_hWnd, SW_SHOWDEFAULT);
}

Window::~Window()
{
	UnregisterClass(m_windowName, m_hInstance);
	DestroyWindow(m_hWnd);
}


bool Window::ProcessMessages()
{
	MSG msg = {};
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))// Tant qu'un message de fenêtre est disponible (voir readme Windows.h)
	{
		if (msg.message == WM_QUIT)// Si le message indique la fermeture de la fenêtre
			return true;

		TranslateMessage(&msg);// Convertie le message à clé virtuelle en messages caractères
		DispatchMessage(&msg);// Transmet le message à une procédure de fenêtre pour réaliser l'action : wc.lpfnWndProc
	}

	return false;
}

/*
* hWnd : handle pour la fenêtre
* msg : message reçu de DispatchMessage()
* wParam : information supplémentaire sur le message
* lParam : information supplémentaire sur le message
*/
LRESULT _stdcall Window::WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_DESTROY)// L'utilisateur appuie sur la croix de la fenêtre
		PostQuitMessage(0);// envoie du message WM_QUIT

	return DefWindowProc(hWnd, msg, wParam, lParam);// Réalise le traitement par défaut du message
}