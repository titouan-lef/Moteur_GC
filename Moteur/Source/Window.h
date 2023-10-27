#pragma once

class Window
{
public:
	Window(const wchar_t* name, UINT width, UINT height, UINT x = CW_USEDEFAULT, UINT y = CW_USEDEFAULT);
	virtual ~Window();
	int Run();

private:
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);// Action a réaliser en fonction du message retourner par ProcessMessages()

	UINT m_width, m_height, m_x, m_y;
	const wchar_t* m_name;// Nom affich� sur la fenêtre
	const wchar_t* m_windowName = L"DirectX 12";// Nom de la fen�tre
	HWND m_hWnd;// Handle de la fen�tre
	HINSTANCE m_hInstance;// Handle de l'instance de la fenêtre
	WindowManager* m_pWinManager = nullptr;

	void Start();
	int Finish(WPARAM wParam);
};