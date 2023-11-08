#include "MyGame.h"
#include <Engine.h>

int main()
{
	Engine::GetInstance()->Init();

	MyGame myGame = MyGame();
	return myGame.Run();
}