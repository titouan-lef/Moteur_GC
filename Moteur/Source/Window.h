#pragma once
#include "EngineException.h"
#include "WindowManager.h"
#include "Keyboard.h"
#include "Mouse.h"

#include <optional>
#include <memory>

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
    static std::optional<int> Run();
    static WindowManager* m_pWinManager;
    static int Finish(WPARAM wParam);
public:
    static wchar_t* convertCharArrayToLPCWSTR(const char* charArray)
    {
        wchar_t* wString = new wchar_t[4096];
        MultiByteToWideChar(CP_ACP, 0, charArray, -1, wString, 4096);
        return wString;
    }

    static std::wstring StringToWString(const std::string& s)
    {
        int len;
        int slength = (int)s.length() + 1;
        len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
        wchar_t* buf = new wchar_t[len];
        MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
        std::wstring r(buf);
        delete[] buf;
        return r;
    }

    static LPCWSTR StringToLPCWSTR(const std::string& s)
    {
        std::wstring temp = StringToWString(s);

        // Allouer un tampon de caractères de large (LPCWSTR) et copier la wstring dedans
        wchar_t* lpcwstr = new wchar_t[temp.length() + 1];
        wcscpy_s(lpcwstr, temp.length() + 1, temp.c_str());

        return lpcwstr;
    }
    //SEULEMENT POUR TEST
    static Keyboard m_kbd;
    static Mouse m_mouse;
    //Keyboard m_kbd;

private:
    static LRESULT CALLBACK HandleMsgSetup(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
    static LRESULT CALLBACK HandleMsgThunk(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)noexcept;
    LRESULT  HandleMsg(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)noexcept;
private:
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);// Action a réaliser en fonction du message retourner par ProcessMessages()

	UINT m_width, m_height, m_x, m_y;
	const wchar_t* m_name;// Nom affich� sur la fenêtre
	const wchar_t* m_windowName = L"DirectX 12";// Nom de la fen�tre
	//SEULEMENT POUR TEST
    static HWND m_hWnd;// Handle de la fen�tre
    //HWND m_hWnd;
    HINSTANCE m_hInstance;// Handle de l'instance de la fenêtre

	void Start();
};


//Error macro
#define EHWND_EXCEPT(hr) Window::HrException(__LINE__, __FILE__, hr);
#define EHWND_LAST_EXCEPT() Window::HrException(__LINE__, __FILE__, GetLastError());
#define EHWND_NOGFX_EXCEPT() Window::NoGfxException( __LINE__,__FILE__ )