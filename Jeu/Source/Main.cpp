#include "MyGame.h"
#include <Engine.h>

int main()
{
	Engine* e = new Engine();
	Engine::Init();

	MyGame myGame;
	return myGame.Run();
}