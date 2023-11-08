#pragma once
#include "M_Input.h"
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
    M_Input* m_input;
    std::map<Direction, bool> m_directionStates;
};