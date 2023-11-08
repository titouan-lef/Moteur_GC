#pragma once
#include "Input.h"
#include <map>


class Controller {
public:
    enum class Direction {
        Up,
        Down,
        Left,
        Right
    };

    Controller();

    void Update();
    bool IsMoving(Direction dir) const;

private:
    Input* m_input;
    std::map<Direction, bool> m_directionStates;
};