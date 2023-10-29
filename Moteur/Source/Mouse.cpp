#include "Mouse.h"
#include <Windows.h>

// Obtient les coordonn�es du curseur de la souris sous forme de paire (x, y)
std::pair<int, int> Mouse::GetPos() const noexcept
{
    return { x, y };
}

// Obtient la coordonn�e x du curseur de la souris
int Mouse::GetPosX() const noexcept
{
    return x;
}

// Obtient la coordonn�e y du curseur de la souris
int Mouse::GetPosY() const noexcept
{
    return y;
}

// V�rifie si le curseur de la souris est � l'int�rieur de la fen�tre
bool Mouse::IsInWindow() const noexcept
{
    return isInWindow;
}

// V�rifie si le bouton gauche de la souris est enfonc�
bool Mouse::LeftIsPressed() const noexcept
{
    return leftIsPressed;
}

// V�rifie si le bouton droit de la souris est enfonc�
bool Mouse::RightIsPressed() const noexcept
{
    return rightIsPressed;
}

// Lit et renvoie le prochain �v�nement de la souris dans la file d'�v�nements
Mouse::Event Mouse::Read() noexcept
{
    if (buffer.size() > 0u) {
        Mouse::Event e = buffer.front();
        buffer.pop();
        return e;
    }
    else {
        return Mouse::Event();
    }
}

// Vide la file d'�v�nements de la souris
void Mouse::Flush() noexcept
{
    buffer = std::queue<Event>();
}

// M�thode pour g�rer le mouvement de la souris
void Mouse::OnMouseMove(int newx, int newy) noexcept
{
    x = newx;
    y = newy;

    buffer.push(Mouse::Event(Mouse::Event::Type::Move, *this));
    TrimBuffer();
}

// M�thode pour g�rer la sortie du curseur de la souris de la fen�tre
void Mouse::OnMouseLeave() noexcept
{
    isInWindow = false;
    buffer.push(Mouse::Event(Mouse::Event::Type::Leave, *this));
    TrimBuffer();
}

// M�thode pour g�rer l'entr�e du curseur de la souris dans la fen�tre
void Mouse::OnMouseEnter() noexcept
{
    isInWindow = true;
    buffer.push(Mouse::Event(Mouse::Event::Type::Enter, *this));
    TrimBuffer();
}

// M�thodes pour g�rer le clic du bouton gauche de la souris
void Mouse::OnLeftPressed(int x, int y) noexcept
{
    leftIsPressed = true;
    buffer.push(Mouse::Event(Mouse::Event::Type::LPress, *this));
    TrimBuffer();
}

void Mouse::OnLeftReleased(int x, int y) noexcept
{
    leftIsPressed = false;
    buffer.push(Mouse::Event(Mouse::Event::Type::LRelease, *this));
    TrimBuffer();
}

// M�thodes pour g�rer le clic du bouton droit de la souris
void Mouse::OnRightPressed(int x, int y) noexcept
{
    rightIsPressed = true;
    buffer.push(Mouse::Event(Mouse::Event::Type::RPress, *this));
    TrimBuffer();
}

void Mouse::OnRightReleased(int x, int y) noexcept
{
    rightIsPressed = false;
    buffer.push(Mouse::Event(Mouse::Event::Type::RRelease, *this));
    TrimBuffer();
}

// M�thodes pour g�rer le d�filement de la souris
void Mouse::OnWheelUp(int x, int y) noexcept
{
    buffer.push(Mouse::Event(Mouse::Event::Type::WheelUp, *this));
    TrimBuffer();
}

void Mouse::OnWheelDown(int x, int y) noexcept
{
    buffer.push(Mouse::Event(Mouse::Event::Type::WheelDown, *this));
    TrimBuffer();
}

void Mouse::OnWheelDelta(int x, int y, int delta) noexcept
{
    wheelDeltaCarry += delta;

    while (wheelDeltaCarry >= WHEEL_DELTA) {
        wheelDeltaCarry -= WHEEL_DELTA;
        OnWheelUp(x, y);
    }
    while (wheelDeltaCarry <= -WHEEL_DELTA) {
        wheelDeltaCarry += WHEEL_DELTA;
        OnWheelDown(x, y);
    }
}

// M�thode pour r�duire la taille de la file d'�v�nements
void Mouse::TrimBuffer() noexcept
{
    while (buffer.size() > bufferSize) {
        buffer.pop();
    }
}
