#pragma once
#include "framwork.h"

class WindowManager;

class Window
{
public:
	Window(const wchar_t* name, UINT width, UINT height, UINT x = CW_USEDEFAULT, UINT y = CW_USEDEFAULT);
	~Window();
	static bool ProcessMessages();// Gérer les messages de la fenêtre (agrandir, quitter, ...)

private:
	static LRESULT _stdcall WindowProc(HWND _hWnd, UINT _msg, WPARAM _wParam, LPARAM _lParam);// Action à réaliser en fonction du message retourner par ProcessMessages()

	int m_width, m_height;
	const wchar_t* m_name;
	const wchar_t* m_windowName = L"DirectX 12";// Nom de la fenêtre (et non le nom affiché sur la fenêtre)
	HWND m_hWnd;// Handle de la fenêtre
	HINSTANCE m_hInstance;// Handle de l'instance de la fenêtre

	WindowManager* m_pWinManager = nullptr;
};