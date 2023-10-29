#include "Mouse.h"
#include <Windows.h>

// Obtient les coordonnées du curseur de la souris sous forme de paire (x, y)
std::pair<int, int> Mouse::GetPos() const noexcept
{
    return { x, y };
}

// Obtient la coordonnée x du curseur de la souris
int Mouse::GetPosX() const noexcept
{
    return x;
}

// Obtient la coordonnée y du curseur de la souris
int Mouse::GetPosY() const noexcept
{
    return y;
}

// Vérifie si le curseur de la souris est à l'intérieur de la fenêtre
bool Mouse::IsInWindow() const noexcept
{
    return isInWindow;
}

// Vérifie si le bouton gauche de la souris est enfoncé
bool Mouse::LeftIsPressed() const noexcept
{
    return leftIsPressed;
}

// Vérifie si le bouton droit de la souris est enfoncé
bool Mouse::RightIsPressed() const noexcept
{
    return rightIsPressed;
}

// Lit et renvoie le prochain événement de la souris dans la file d'événements
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

// Vide la file d'événements de la souris
void Mouse::Flush() noexcept
{
    buffer = std::queue<Event>();
}

// Méthode pour gérer le mouvement de la souris
void Mouse::OnMouseMove(int newx, int newy) noexcept
{
    x = newx;
    y = newy;

    buffer.push(Mouse::Event(Mouse::Event::Type::Move, *this));
    TrimBuffer();
}

// Méthode pour gérer la sortie du curseur de la souris de la fenêtre
void Mouse::OnMouseLeave() noexcept
{
    isInWindow = false;
    buffer.push(Mouse::Event(Mouse::Event::Type::Leave, *this));
    TrimBuffer();
}

// Méthode pour gérer l'entrée du curseur de la souris dans la fenêtre
void Mouse::OnMouseEnter() noexcept
{
    isInWindow = true;
    buffer.push(Mouse::Event(Mouse::Event::Type::Enter, *this));
    TrimBuffer();
}

// Méthodes pour gérer le clic du bouton gauche de la souris
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

// Méthodes pour gérer le clic du bouton droit de la souris
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

// Méthodes pour gérer le défilement de la souris
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

// Méthode pour réduire la taille de la file d'événements
void Mouse::TrimBuffer() noexcept
{
    while (buffer.size() > bufferSize) {
        buffer.pop();
    }
}
