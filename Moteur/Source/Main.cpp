#include "framwork.h"
#include "App.h"

class GameObject;

_Use_decl_annotations_
int WINAPI WinMain(HINSTANCE m_hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
	//GameObject go1(0, 0, 0, 0.5f, 0.5f, 0.5f);
	//GameObject go2(-0.5f, 0, 0, 0.25f, 0.25f, 0.25f);

    try {
        // Initialiser la fen�tre


        return App{}.Go();

    }
    catch (const EngineException& e) {
        MessageBox(nullptr, WindowManager::convertCharArrayToLPCWSTR(e.what()), WindowManager::convertCharArrayToLPCWSTR(e.GetType()), MB_OK | MB_ICONEXCLAMATION);
    }
    catch (const std::exception& e) {
        MessageBox(nullptr, WindowManager::convertCharArrayToLPCWSTR(e.what()), WindowManager::convertCharArrayToLPCWSTR("Standard Exception"), MB_OK | MB_ICONEXCLAMATION);
    }
    catch (...) {
        MessageBox(nullptr, WindowManager::convertCharArrayToLPCWSTR("No Details Available"), WindowManager::convertCharArrayToLPCWSTR("Unknown Exception"), MB_OK | MB_ICONEXCLAMATION);
    }
    return -1;
}