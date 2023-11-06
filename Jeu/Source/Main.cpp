#include "MyGame.h"
#include <Engine.h>
#include <Camera.h>

int main()
{
	Engine();

	MyGame myGame = MyGame();
	return myGame.Run();
}