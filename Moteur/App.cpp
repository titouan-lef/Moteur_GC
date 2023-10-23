#include "App.h"
#include <sstream>
#include <iomanip>
App::App() : wnd(MyWindow::convertCharArrayToLPCWSTR("Triangle"), 800, 600)
{
    // Constructeur de l'application : Initialise la fen�tre "wnd" avec le nom "Triangle" et les dimensions 800x600.
}

int App::Go()
{
    while (true)
    {
        // Boucle principale de l'application.

        // Processus de tous les messages Windows (y compris la fermeture de la fen�tre).
        if (const auto ecode = MyWindow::ProcessMessages()) {
            return *ecode;
        }

        // Effectue une frame de rendu.
        DoFrame();
    }
}

void App::DoFrame()
{
    // Fonction de rendu pour une frame.

    // Calcule une valeur "c" bas�e sur le temps (oscillation entre 0 et 1).
    const float c = sin(timer.Peek()) / 2.0f + 0.5f;

    // Efface le tampon de rendu (buffer) avec la couleur de fond "c" (teinte bleue variable).
    wnd.Gfx().ClearBuffer(c, c, 1.0f);

    // Dessine un triangle en utilisant la classe "Graphics" associ�e � la fen�tre "wnd".
    wnd.Gfx().DrawTestTriangle();

    // Termine la frame de rendu, ce qui affiche le r�sultat � l'�cran.
    wnd.Gfx().EndFrame();
}
