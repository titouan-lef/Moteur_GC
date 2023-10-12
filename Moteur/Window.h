#pragma once
#include <Windows.h>

class Window
{
public:
	Window(const char* _name, int _width, int _height, int x = CW_USEDEFAULT, int y = CW_USEDEFAULT);
	~Window();
	static bool ProcessMessages();// G�rer les messages de la fen�tre (agrandir, quitter, ...)

private:
	static LRESULT _stdcall WindowProc(HWND _hWnd, UINT _msg, WPARAM _wParam, LPARAM _lParam);// Action � r�aliser en fonction du message retourner par ProcessMessages()

	int width, height;
	const char* windowName = "DirectX 12";
	HWND hWnd;// Handle de la fen�tre
	HINSTANCE hInstance;// Handle de l'instance de la fen�tre
};