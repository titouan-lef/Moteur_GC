#pragma once
#include <Windows.h>

class Window
{
public:
	Window(const char* _name, int _width, int _height, int _x = CW_USEDEFAULT, int _y = CW_USEDEFAULT);
	~Window();
	static bool ProcessMessages();// Gérer les messages de la fenêtre (agrandir, quitter, ...)

private:
	static LRESULT _stdcall WindowProc(HWND _hWnd, UINT _msg, WPARAM _wParam, LPARAM _lParam);// Action à réaliser en fonction du message retourner par ProcessMessages()

	int width, height;
	const char* windowName = "DirectX 12";// Nom de la fenêtre (et non le nom affiché sur la fenêtre)
	HWND hWnd;// Handle de la fenêtre
	HINSTANCE hInstance;// Handle de l'instance de la fenêtre

};