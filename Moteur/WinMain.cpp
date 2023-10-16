#include "Window.h"

/*
* _hInstance : handle de notre application
* _hPrevInstance : handle permettant de gérer les différentes instances de notre application (ici toujours nulle car nous avons qu'une seule instance)
* _lpCmdLine : gère la ligne de commande de l'application
* _nCmdShow : contrôle la façon dont la fenêtre sera affichée
*/
int _stdcall WinMain(HINSTANCE _hInstance, HINSTANCE _hPrevInstance, LPSTR _lpCmdLine, int _nCmdShow)
{
	Window window("DirectX 12", 800, 600);

	while (true)
	{
		if (Window::ProcessMessages())
			return 0;


		window.Gfx().ClearBuffer(1, 1, 1);
		window.Gfx().EndFrame();
	}
}