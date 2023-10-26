#include "MyWindow.h"
#include <sstream>
#include "resource.h"

// Initialisation de la classe de fen�tre (WindowClass)
MyWindow::WindowClass MyWindow::WindowClass::wndClass;

// Fonction pour obtenir le nom de la classe de fen�tre
const wchar_t* MyWindow::WindowClass::GetName() noexcept
{
    return convertCharArrayToLPCWSTR(wndClassName);
}

// Fonction pour obtenir l'instance de l'application
HINSTANCE MyWindow::WindowClass::GetInstance() noexcept
{
    return wndClass.hInst;
}

// Constructeur de la classe de fen�tre (WindowClass)
MyWindow::WindowClass::WindowClass() noexcept :
    hInst(GetModuleHandle(nullptr))
{
    // Configuration de la structure de classe de fen�tre (WNDCLASSEX)
    WNDCLASSEX wc = { 0 };
    wc.cbSize = sizeof(wc);
    wc.style = CS_OWNDC;
    wc.lpfnWndProc = HandleMsgSetup;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = GetInstance();
    wc.hIcon = static_cast<HICON>(LoadImage(GetInstance(), MAKEINTRESOURCE(IDB_PNG1), IMAGE_ICON, 32, 32, 0));
    wc.hCursor = nullptr;
    wc.hbrBackground = nullptr;
    wc.lpszClassName = GetName();
    wc.lpszMenuName = nullptr;

    // Enregistrement de la classe de fen�tre
    RegisterClassEx(&wc);
}

// Destructeur de la classe de fen�tre (WindowClass)
MyWindow::WindowClass::~WindowClass()
{
    // D�senregistrement de la classe de fen�tre
    UnregisterClass(convertCharArrayToLPCWSTR(wndClassName), GetInstance());
}

// Constructeur de la fen�tre (MyWindow)
MyWindow::MyWindow(const wchar_t* name, int width, int height) : width(width), height(height)
{
    // D�finition de la taille/zone de la fen�tre
    RECT wr;
    wr.left = 100;
    wr.right = width + wr.left;
    wr.top = 100;
    wr.bottom = height + wr.top;
    if (AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE) == 0) {
        throw "AjustWIndow";
    }

    // Cr�ation de la fen�tre
    hwnd = CreateWindow(
        WindowClass::GetName(), name,
        WS_CAPTION | WS_MINIMIZE | WS_SYSMENU,
        CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top,
        nullptr, nullptr, WindowClass::GetInstance(), this);

    // Gestion des erreurs
    if (hwnd == nullptr) {
        throw "hwnd null";
    }

    // Affichage de la fen�tre
    ShowWindow(hwnd, SW_SHOWDEFAULT);

    // Initialisation de la gestion graphique (Graphics)
    pGfx = std::make_unique<Graphics>(hwnd);
}

// Destructeur de la fen�tre (MyWindow)
MyWindow::~MyWindow()
{
    DestroyWindow(hwnd);
}

// D�finition du titre de la fen�tre
void MyWindow::SetTitle(const std::string& title)
{
    if (SetWindowText(hwnd, convertCharArrayToLPCWSTR(title.c_str())) == 0) {
        throw "Error Title";
    }
}

// Traitement des messages Windows et gestion de la boucle d'�v�nements
std::optional<int> MyWindow::ProcessMessages()
{
    MSG msg;
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
    {
        if (msg.message == WM_QUIT) {
            return msg.wParam;
        }

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return {};
}

// Obtention de l'objet Graphics associ� � la fen�tre
Graphics& MyWindow::Gfx()
{
    return *pGfx;
}

// Fonction de gestion de message utilis�e lors de la cr�ation de la fen�tre
LRESULT MyWindow::HandleMsgSetup(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
    if (msg == WM_NCCREATE) {
        // Extraction du pointeur vers MyWindow depuis la structure CREATESTRUCTW
        const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
        MyWindow* const pWnd = static_cast<MyWindow*>(pCreate->lpCreateParams);
        // Configuration des donn�es utilisateur g�r�es par WinAPI pour stocker le pointeur vers la classe de fen�tre
        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
        // Configuration de la fonction de gestion de message (msg proc) pour utiliser HandleMsgThunk apr�s la configuration initiale
        SetWindowLongPtr(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&MyWindow::HandleMsgThunk));
        // Envoi du message
        return pWnd->HandleMsg(hwnd, msg, wParam, lParam);
    }
    // Si un message est re�u avant WM_NCCREATE, utilisez la fonction de gestion de message par d�faut
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

// Fonction de gestion de message "d'assistance" utilis�e pour la gestion des messages Windows
LRESULT MyWindow::HandleMsgThunk(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
    MyWindow* const pWnd = reinterpret_cast<MyWindow*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    return pWnd->HandleMsg(hwnd, msg, wParam, lParam);
}

// Fonction de gestion de message principale pour la fen�tre
LRESULT MyWindow::HandleMsg(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
    switch (msg)
    {
    case WM_CLOSE:
        // Envoi un message de fermeture de l'application
        PostQuitMessage(0);
        return 0;
    }
    // Utilise la fonction de gestion de message par d�faut pour les messages non g�r�s
    return DefWindowProc(hwnd, msg, wParam, lParam);
}
