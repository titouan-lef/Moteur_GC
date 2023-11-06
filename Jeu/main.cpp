#include <Window.h>
#include <Cube.h>
#include <WindowManager.h>

int main()
{
	Window window(L"DirectX 12", 800, 600);

	Cube* c = new Cube();

	while (true)
	{
		if (Window::ProcessMessages())
			return 0;

		window.m_pWinManager->OnUpdate();
		window.m_pWinManager->OnRender();

		//Update();
		//Render();
	}
}

void Update()
{

}

void Render()
{
	
}