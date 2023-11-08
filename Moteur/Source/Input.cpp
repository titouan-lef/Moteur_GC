#include "Input.h"

Input::Input()
{
    keystates.reset();
    prevKeystates.reset();
    m_mouse = nullptr;
}
void Input::Update()
{
    prevKeystates = keystates;
    for ( int i = 0; i < 256; ++i)
    {
        keystates[i] = GetAsyncKeyState(i) & 0x8000;
    }
}



bool Input::KeyIsPressed(unsigned char keycode) const
{
    return keystates[keycode];
}

bool Input::KeyIsPressedOnce(unsigned char keycode) const
{
    return keystates[keycode] && !prevKeystates[keycode];
}

bool Input::KeyIsReleased(unsigned char keycode) const
{
    return !keystates[keycode] && prevKeystates[keycode];
}
