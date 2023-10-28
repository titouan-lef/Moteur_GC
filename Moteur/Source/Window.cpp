#include "framwork.h"
#include "Window.h"
#include <sstream>


WindowManager* Window::m_pWinManager = nullptr;
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
LRESULT Window::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	if (msg == WM_NCCREATE) {
		//extrait ptr à Window
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);
		//set winapi managed user data to store ptr to win class
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
		//set msg proc to normal hangler now that setup is finished
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk));
		//envoie du msg
		return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
	}
	//si on recoie un msg avant WM NCCREATE on utilise le default
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT Window::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)noexcept
{
	Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
}

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
	}
	return 0;


	case WM_PAINT:
		if (pWinManager)
		{
			pWinManager->OnUpdate();
			pWinManager->OnRender();
		}
		return 0;

	case WM_CLOSE:// L'utilisateur appuie sur la croix de la fen�tre
		PostQuitMessage(0);// (voir readme Windows.h)
		return 0;
#pragma region KEYBOARD
//		//clear keystates
//	case WM_KILLFOCUS:
//		m_kbd.ClearState();
//		break;
//		/***********************Keyboard**********************/
//	case WM_KEYDOWN:
//	case WM_SYSKEYDOWN:
//		if (!(lParam & 0x40000000) || m_kbd.AutoRepeatIsEnable()) {
//
//			m_kbd.OnKeyPressed(static_cast<unsigned char>(wParam));
//		}
//		break;
//	case WM_KEYUP:
//	case WM_SYSKEYUP:
//		m_kbd.OnKeyReleased(static_cast<unsigned char>(wParam));
//		break;
//	case WM_CHAR:
//		m_kbd.OnChar(static_cast<unsigned char>(wParam));
//		break;
//
		/***********************END Keyboard**********************/
#pragma endregion KEYBOARD
#pragma region MOUSE
	//	/***********************Mouse**********************/
	//case WM_MOUSEMOVE:
	//{
	//	const POINTS pt = MAKEPOINTS(lParam);
	//	//Cas dans la région de la window
	//	if (pt.x >= 0 && pt.x < m_width && pt.y >= 0 && pt.y < m_height) {
	//
	//		m_mouse.OnMouseMove(pt.x, pt.y);
	//		if (!m_mouse.IsInWindow()) {
	//			SetCapture(m_hWnd);
	//			m_mouse.OnMouseEnter();
	//		}
	//	}
	//	//Cas contraire
	//	else {
	//		if (wParam & (MK_LBUTTON | MK_RBUTTON)) {
	//			m_mouse.OnMouseMove(pt.x, pt.y);
	//		}
	//		else {
	//			ReleaseCapture();
	//			m_mouse.OnMouseLeave();
	//		}
	//	}
	//	break;
	//}
	//case WM_LBUTTONDOWN:
	//{
	//	const POINTS pt = MAKEPOINTS(lParam);
	//	m_mouse.OnLeftPressed(pt.x, pt.y);
	//	break;
	//}
	//case WM_RBUTTONDOWN:
	//{
	//	const POINTS pt = MAKEPOINTS(lParam);
	//	m_mouse.OnLeftPressed(pt.x, pt.y);
	//	break;
	//}
	//case WM_LBUTTONUP:
	//{
	//	const POINTS pt = MAKEPOINTS(lParam);
	//	m_mouse.OnLeftReleased(pt.x, pt.y);
	//	break;
	//}
	//case WM_RBUTTONUP:
	//{
	//	const POINTS pt = MAKEPOINTS(lParam);
	//	m_mouse.OnRightReleased(pt.x, pt.y);
	//	break;
	//}
	//case WM_MOUSEWHEEL:
	//{
	//	const POINTS pt = MAKEPOINTS(lParam);
	//	const int delta = GET_WHEEL_DELTA_WPARAM(wParam);
	//	m_mouse.OnWheelDelta(pt.x, pt.y, delta);
	//	break;
	//}
	///***********************END MOUSE**********************/

#pragma endregion MOUSE
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}



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
		WindowManager::convertCharArrayToLPCWSTR(pMsgBuf), 0, nullptr
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