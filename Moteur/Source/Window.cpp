#include "framwork.h"
#include "WindowManager.h"
#include "Window.h"

/*
* x et y : coordon�es (x, y) du coin sup�rieur gauche de la fen�tre
*/
Window::Window(const wchar_t* name, UINT width, UINT height, UINT x, UINT y)
	:
	m_name(name),
	m_width(width),
	m_height(height),
	m_x(x),
	m_y(y),
	m_hInstance(GetModuleHandle(nullptr))// (voir readme Windows.h)
{
	Start();
}

Window::~Window()
{
	delete m_pWinManager;

	UnregisterClass(m_windowName, m_hInstance);
	DestroyWindow(m_hWnd);
}

void Window::Start()
{
	m_pWinManager = new WindowManager(m_width, m_height);

	// Cr�ation d'une classe de fen�tre
	WNDCLASS wc = {};
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = m_hInstance;
	wc.lpszClassName = m_windowName;
	RegisterClass(&wc);

	// Agrandissement de la taille de la fen�tre pour prendre en compte son style et ses bordures
	RECT winRect{};
	winRect.left = m_x;
	winRect.right = m_x + m_width;
	winRect.top = m_y;
	winRect.bottom = m_y + m_height;
	AdjustWindowRect(&winRect, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);// (voir readme Windows.h)

	// Cr�ation de la fen�tre
	m_hWnd = CreateWindow(
		m_windowName, m_name,
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		m_x, m_y, winRect.right - winRect.left, winRect.bottom - winRect.top,
		nullptr, nullptr, m_hInstance, m_pWinManager
	);

	m_pWinManager->OnInit(m_width, m_height, m_hWnd);

	// Affichage de la fen�tre
	ShowWindow(m_hWnd, SW_SHOWDEFAULT);
}

int Window::Run()
{
	// Main sample loop.
	MSG msg = {};
	while (msg.message != WM_QUIT)// Tant que le message n'indique pas la fermeture de la fen�tre
	{
		// Process any messages in the queue.
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))// Si un message de fen�tre est disponible (voir readme Windows.h)
		{
			TranslateMessage(&msg);// (voir readme Windows.h)
			DispatchMessage(&msg);// (voir readme Windows.h)
		}
	}

	return Finish(msg.wParam);
}

int Window::Finish(WPARAM wParam)
{
	m_pWinManager->OnDestroy();

	// Return this part of the WM_QUIT message to Windows.
	return static_cast<char>(wParam);
}

/*
* _hWnd : handle pour la fen�tre
* _msg : message re�u de DispatchMessage()
* _wParam : information suppl�mentaire sur le message
* _lParam : information suppl�mentaire sur le message
*/
LRESULT CALLBACK Window::WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	WindowManager* pWinManager = reinterpret_cast<WindowManager*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

	switch (msg)
	{
	case WM_CREATE:
	{
		// Sauvegarde le WindowManager* passé dans CreateWindow
		LPCREATESTRUCT pCreateStruct = reinterpret_cast<LPCREATESTRUCT>(lParam);
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pCreateStruct->lpCreateParams));
	}
	return 0;

	case WM_KEYDOWN:
		if (pWinManager)
			pWinManager->OnKeyDown(static_cast<UINT8>(wParam));

		return 0;

	case WM_KEYUP:
		if (pWinManager)
			pWinManager->OnKeyUp(static_cast<UINT8>(wParam));

		return 0;

	case WM_PAINT:
		if (pWinManager)
		{
			pWinManager->OnUpdate();
			pWinManager->OnRender();
		}
		return 0;

	case WM_DESTROY:// L'utilisateur appuie sur la croix de la fen�tre
		PostQuitMessage(0);// (voir readme Windows.h)
		return 0;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);// (voir readme Windows.h)
}
