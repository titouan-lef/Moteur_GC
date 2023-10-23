#include "Window.h"
/*
* _x et _y : coordon�es (x, y) du coin sup�rieur gauche de la fen�tre
*/
Window::Window(const char* _name, int _width, int _height, int _x, int _y)
	:
	width(_width),
	height(_height),
	hInstance(GetModuleHandle(nullptr))// (voir readme Windows.h)
{
	// Cr�ation d'une classe de fen�tre
	WNDCLASS wc = {};
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = windowName;
	//RegisterClass(&wc);

	// Si erreur lors de la cr�ation de la classe fen�tre
	if (!RegisterClass(&wc)) {
		MessageBeep(1);
		MessageBoxA(0, "[Error] Register class", "Fatal Error", MB_OK);
	}

	// Agrandissement de la taille de la fen�tre pour prendre en compte son style et ses bordures
	RECT winRect{};
	winRect.left = _x;
	winRect.right = _x + width;
	winRect.top = _y;
	winRect.bottom = _y + height;
	AdjustWindowRect(&winRect, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);// (voir readme Windows.h)

	// Cr�ation de la fen�tre
	hWnd = CreateWindow(
		windowName, _name,
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		_x, _y, winRect.right - winRect.left, winRect.bottom - winRect.top,
		nullptr, nullptr, hInstance, this
	);

	// Si erreur lors de la cr�ation de la fen�tre
	if (!hWnd) {
		MessageBeep(1);
		MessageBoxA(0, "[Error] Creating a window failed!", "Fatal Error", MB_OK | MB_ICONERROR);
	}

	// Affichage de la fen�tre
	ShowWindow(hWnd, SW_SHOWDEFAULT);

	//Triangle t = Triangle(0, 0, 0, 100, 0, 0, 0, 100, 0);
	//Triangle t = Triangle(0, 0, 0, 0, 100, 0, 100, 0, 0);
	pGfx = new Graphic(hWnd);
}

Window::~Window()
{
	delete pGfx;

	UnregisterClass(windowName, hInstance);
	DestroyWindow(hWnd);
}


bool Window::ProcessMessages()
{
	MSG msg = {};
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))// Tant qu'un message de fen�tre est disponible (voir readme Windows.h)
	{
		if (msg.message == WM_QUIT)// Si le message indique la fermeture de la fen�tre
			return true;

		TranslateMessage(&msg);// (voir readme Windows.h)
		DispatchMessage(&msg);// (voir readme Windows.h)
	}

	return false;
}

Graphic& Window::Gfx()
{
	return *pGfx;
}

/*
* _hWnd : handle pour la fen�tre
* _msg : message re�u de DispatchMessage()
* _wParam : information suppl�mentaire sur le message
* _lParam : information suppl�mentaire sur le message
*/
LRESULT _stdcall Window::WindowProc(HWND _hWnd, UINT _msg, WPARAM _wParam, LPARAM _lParam)
{
	switch (_msg)
	{
	case WM_DESTROY:// L'utilisateur appuie sur la croix de la fen�tre
		PostQuitMessage(0);// (voir readme Windows.h)
		break;
	}

	return DefWindowProc(_hWnd, _msg, _wParam, _lParam);// (voir readme Windows.h)
}