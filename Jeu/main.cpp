#include <Window.h>
#include <Cube.h>

int main()
{
	Window window(L"DirectX 12", 800, 600);

	Cube* c = new Cube();

	while (true)
	{
		if (Window::ProcessMessages())
			return 0;

		Update();
		Render();
	}
}

void Update()
{

}

void Render()
{
	
}