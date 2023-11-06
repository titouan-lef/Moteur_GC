#include "App.h"
#include <sstream>
#include <iomanip>
App::App() : wnd(L"Triangle", 800, 600)
{
    // Constructeur de l'application : Initialise la fenêtre "wnd" avec le nom "Triangle" et les dimensions 800x600.
}

int App::Go()
{
    while (true)
    {
        // Boucle principale de l'application.

        // Processus de tous les messages Windows (y compris la fermeture de la fenêtre).
        if (const auto ecode = Window::Run()) {
            return ecode;
        }

    }
}