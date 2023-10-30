﻿#include "framwork.h"
#include "Window.h"
#include <sstream>
#include "MyException.h"


WindowManager* Window::m_pWinManager = nullptr;


HWND Window::m_hWnd;


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
	wc.lpfnWndProc = HandleMsgSetup;
	wc.hInstance = m_hInstance;
	wc.lpszClassName = m_windowName;
	RegisterClass(&wc);

	// Agrandissement de la taille de la fen�tre pour prendre en compte son style et ses bordures
	RECT winRect{};
	winRect.left = m_x;
	winRect.right = m_x + m_width;
	winRect.top = m_y;
	winRect.bottom = m_y + m_height;
	if (AdjustWindowRect(&winRect, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE)==0) {
		throw EHWND_LAST_EXCEPT();
	};// (voir readme Windows.h)
	
	// Cr�ation de la fen�tre
	m_hWnd = CreateWindow(
		m_windowName, m_name,
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		m_x, m_y, winRect.right - winRect.left, winRect.bottom - winRect.top,
		nullptr, nullptr, m_hInstance, m_pWinManager
	);
	//Error
	if (m_hWnd == nullptr) {
		throw EHWND_LAST_EXCEPT();
	}

	m_pWinManager->OnInit(m_width, m_height, m_hWnd);

	// Affichage de la fen�tre
	ShowWindow(m_hWnd, SW_SHOWDEFAULT);
}

std::optional<int> Window::Run()
{
	// Main sample loop.
	MSG msg = {};
	while (msg.message != WM_QUIT)// Tant que le message n'indique pas la fermeture de la fen�tre
	{
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) {
				return msg.wParam;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}



		WindowManager* pWinManager = reinterpret_cast<WindowManager*>(GetWindowLongPtr(m_hWnd, GWLP_USERDATA));
		pWinManager->OnUpdate();
		pWinManager->OnRender();
	}
	

	return Finish(msg.wParam);
}

int Window::Finish(WPARAM wParam)
{
	m_pWinManager->OnDestroy();

	// Return this part of the WM_QUIT message to Windows.
	return static_cast<char>(wParam);
}


// Handle the initial setup of the window message handler
LRESULT Window::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
	{
		if (msg == WM_NCCREATE) {
			// Extract the pointer to the Window class from lParam
			const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
			Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);
			// Set the WinAPI managed user data to store a pointer to the Window class
			SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
			// Set the message procedure to the normal handler now that setup is finished
			SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk));
			// Forward the message to the HandleMsg function of the Window class
			return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
		}
		// If a message is received before WM_NCCREATE, use the default message handler
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

// Thunk function to handle window messages and forward them to the actual Window class
LRESULT Window::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
	{
		// Retrieve a pointer to the Window class from user data
		Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
		// Forward the message to the HandleMsg function of the Window class
		return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
	}

// Handle window messages for the Window class
LRESULT Window::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)noexcept
{
	WindowManager* pWinManager = reinterpret_cast<WindowManager*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	switch (msg)
	{
		

	case WM_CREATE:
	{
		// Sauvegarde le WindowManager* passé dans CreateWindow
		LPCREATESTRUCT pCreateStruct = reinterpret_cast<LPCREATESTRUCT>(lParam);
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pCreateStruct->lpCreateParams));
		break;
	}
	//return 0;

	

	//case WM_PAINT:
	//	if (pWinManager)
	//	{
	//		pWinManager->OnUpdate();
	//		pWinManager->OnRender();
	//	}
	//	return 0;

	case WM_DESTROY:// L'utilisateur appuie sur la croix de la fen�tre
		PostQuitMessage(0);// (voir readme Windows.h)
		return 0;

	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}


#pragma region EXCEPTION

//Exception Stuff
Window::HrException::HrException(int line, const char* file, HRESULT hr) noexcept : Exception(line, file), hr(hr)
{
}
const char* Window::HrException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "[Error Code] 0x" << std::hex << std::uppercase << GetErrorCode()
		<< std::dec << " (" << (unsigned long)GetErrorCode() << ")" << std::endl
		<< "[Description] " << GetErrorDescription() << std::endl
		<< GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* Window::HrException::GetType() const noexcept
{
	return "Engine Window Exception";
}

std::string Window::Exception::TranslateErrorCode(HRESULT hr) noexcept
{
	char* pMsgBuf = nullptr;
	DWORD nMsgLen = FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		MyException::convertCharArrayToLPCWSTR(pMsgBuf), 0, nullptr
	);
	if (nMsgLen == 0) {
		return "Unidentified error code";
	}
	if (pMsgBuf == 0) {
		return "pMsgBuf = 0 in Window::Eception";
	}
	std::string errorString = pMsgBuf;
	LocalFree(pMsgBuf);
	return errorString;
}

HRESULT Window::HrException::GetErrorCode() const noexcept
{
	return hr;
}

const char* Window::NoGfxException::GetType() const noexcept
{
	return "Window Exception [No Graphics]";
}


std::string Window::HrException::GetErrorDescription() const noexcept
{
	return Exception::TranslateErrorCode(hr);
}
#pragma endregion EXCEPTION