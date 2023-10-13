#include "Window.h"

/*
* _x et _y : coordonées (x, y) du coin supérieur gauche de la fenêtre
*/

Window::Window(const char* _name, int _width, int _height, int _x, int _y)
	:
	width(_width),
	height(_height),
	hInstance(GetModuleHandle(nullptr)) // GetModuleHandle(nullptr)  : handle vers notre application
{

	// Création d'une classe de fenêtre
	WNDCLASS wc = {};
	wc.lpfnWndProc = WindowProc; 
	wc.hInstance = hInstance;
	wc.lpszClassName = windowName;
	wc.hIcon = LoadIcon(wc.hInstance, MAKEINTRESOURCE(IDI_Unity));		//Ajout d'un icon
	RegisterClass(&wc);
	

	// Agrandissement de la taille de la fenêtre pour prendre en compte son style et ses bordures
	RECT winRect{};
	winRect.left = _x;
	winRect.right = _x + width;
	winRect.top = _y;
	winRect.bottom = _y + height;
	AdjustWindowRect(&winRect, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);// (voir readme Windows.h)

	// Création de la fenêtre (voir readme Windows.h)
	hWnd = CreateWindow(
		windowName, _name,
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		_x, _y, winRect.right - winRect.left, winRect.bottom - winRect.top,
		nullptr, nullptr, hInstance, this
	);// (voir readme Windows.h)

	// Affichage de la fenêtre
	ShowWindow(hWnd, SW_SHOWDEFAULT);
}

Window::~Window()
{
	UnregisterClass(windowName, hInstance);
	DestroyWindow(hWnd);
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
* _hWnd : handle pour la fenêtre
* _msg : message reçu de DispatchMessage()
* _wParam : information supplémentaire sur le message
* _lParam : information supplémentaire sur le message
*/
LRESULT _stdcall Window::WindowProc(HWND _hWnd, UINT _msg, WPARAM _wParam, LPARAM _lParam)
{
	switch (_msg)
	{
	case WM_DESTROY:// L'utilisateur appuie sur la croix de la fenêtre
		PostQuitMessage(0);// envoie du message WM_QUIT
		break;
	}

	return DefWindowProc(_hWnd, _msg, _wParam, _lParam);// Réalise le traitement par défaut du message
}