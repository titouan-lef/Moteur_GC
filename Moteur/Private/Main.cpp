#include "header/framwork.h"
#include "header/Window.h"

class GameObject;

_Use_decl_annotations_
int WINAPI WinMain(HINSTANCE m_hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
	Window window(L"DirectX 12", 800, 600);
	GameObject go1(0, 0, 0, 0.5f, 0.5f, 0.5f);
	GameObject go2(-0.5f, 0, 0, 0.25f, 0.25f, 0.25f);

	return window.Run();
}