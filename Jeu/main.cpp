#include "framwork.h"
#include "App.h"
#include "MyException.h"



int main()
{
    try {


        return App{}.Go();
    }
    catch (const EngineException& e) {
        MessageBox(nullptr, MyException::convertCharArrayToLPCWSTR(e.what()), MyException::convertCharArrayToLPCWSTR(e.GetType()), MB_OK | MB_ICONEXCLAMATION);
    }
    catch (const std::exception& e) {
        MessageBox(nullptr, MyException::convertCharArrayToLPCWSTR(e.what()), MyException::convertCharArrayToLPCWSTR("Standard Exception"), MB_OK | MB_ICONEXCLAMATION);
    }
    catch (...) {
        MessageBox(nullptr, MyException::convertCharArrayToLPCWSTR("No Details Available"), MyException::convertCharArrayToLPCWSTR("Unknown Exception"), MB_OK | MB_ICONEXCLAMATION);
    }
    return -1;
}