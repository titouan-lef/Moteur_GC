#pragma once
#include "EngineException.h"
#include "WindowManager.h"

class Window
{
public:
#pragma region EXCEPTION
    class Exception : public EngineException
    {
        using EngineException::EngineException;
    public:
        static std::string TranslateErrorCode(HRESULT hr) noexcept;
    };
    class HrException : public Exception
    {
    public:
        HrException(int line, const char* file, HRESULT hr) noexcept;
        const char* what() const noexcept override;
        const char* GetType() const noexcept override;
        HRESULT GetErrorCode() const noexcept;
        std::string GetErrorDescription() const noexcept;
    private:
        HRESULT hr;
    };
    class NoGfxException : public Exception
    {
    public:
        using Exception::Exception;
        const char* GetType() const noexcept override;
    };


#pragma endregion EXCEPTION
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


//Error macro
#define EHWND_EXCEPT(hr) Window::HrException(__LINE__, __FILE__, hr);
#define EHWND_LAST_EXCEPT() Window::HrException(__LINE__, __FILE__, GetLastError());
#define EHWND_NOGFX_EXCEPT() Window::NoGfxException( __LINE__,__FILE__ )