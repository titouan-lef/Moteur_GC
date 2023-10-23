#pragma once
#include "HeaderWin.h"

#include "Graphics.h"
#include <optional>
#include <memory>
#include <string>
class MyWindow
{
private:
    class WindowClass {

        public:
            static const  wchar_t* GetName()noexcept;
            static HINSTANCE GetInstance() noexcept;
        private:
            WindowClass() noexcept;
            ~WindowClass();
            WindowClass(const WindowClass&) = delete;
            WindowClass& operator =(const WindowClass&) = delete;
            static constexpr const char* wndClassName = "Triangle Window";
            static WindowClass wndClass;
            HINSTANCE hInst;

    };



public:
    MyWindow(const wchar_t* name, int w, int h) ;
    ~MyWindow();
    MyWindow(const MyWindow&) = delete;
    MyWindow& operator = (const MyWindow&) = delete;
    void SetTitle(const std::string& title);
    static std::optional<int> ProcessMessages();
    Graphics& Gfx();
    //juste un convertisseur
    static wchar_t* convertCharArrayToLPCWSTR(const char* charArray)
    {
        wchar_t* wString = new wchar_t[4096];
        MultiByteToWideChar(CP_ACP, 0, charArray, -1, wString, 4096);
        return wString;
    }

private:
    static LRESULT CALLBACK HandleMsgSetup(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
    static LRESULT CALLBACK HandleMsgThunk(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)noexcept;
    LRESULT  HandleMsg(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)noexcept;


public:

private:
    HWND hwnd;
    int width;
    int height;
    std::unique_ptr<Graphics> pGfx;
};

