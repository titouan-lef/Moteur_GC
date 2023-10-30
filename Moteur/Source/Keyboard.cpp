#include "Keyboard.h"

bool Keyboard::KeyIsPressed(unsigned char code)  noexcept
{
  
    return keystates[code];
}



Keyboard::Event Keyboard::ReadKey() noexcept
{
    if (keybuffer.size() > 0u) {
        Keyboard::Event e = keybuffer.front();
        keybuffer.pop();
        return e;
    }
    else {
        return Keyboard::Event();
    }
}

bool Keyboard::KeyIsEmpty() const noexcept
{
    return keybuffer.empty();
}

void Keyboard::FlushKey() noexcept
{
    keybuffer = std::queue<Event>();
}



void Keyboard::OnKeyPressed(unsigned char keycode) noexcept
{
    m_timer.Mark();
    keystates[keycode] = true;
    keybuffer.push(Keyboard::Event(Keyboard::Event::Type::Press, keycode));
    TrimBuffer(keybuffer);
}

void Keyboard::OnKeyDown(unsigned char keycode) noexcept
{
    m_elaplsedTime = m_timer.Peek();

    if (m_elaplsedTime <= 0.1) {
        keystates[keycode] = false;
        keybuffer.push(Keyboard::Event(Keyboard::Event::Type::Down, keycode));
        TrimBuffer(keybuffer);
    }
    m_timer.Mark();
}

void Keyboard::OnKeyReleased(unsigned char keycode) noexcept
{
    keystates[keycode] = false;
    keybuffer.push(Keyboard::Event(Keyboard::Event::Type::Release, keycode));
    TrimBuffer(keybuffer);
}



void Keyboard::ClearState() noexcept
{
    keystates.reset();
}

template<typename T>
void Keyboard::TrimBuffer(std::queue<T>& buffer) noexcept {
    while (buffer.size() > bufferSize) {
        buffer.pop();
    }
}