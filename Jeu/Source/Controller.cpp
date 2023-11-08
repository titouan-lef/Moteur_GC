#include "Controller.h"




Controller::Controller() {

    m_input = new M_Input();
    // Initialise toutes les directions à faux
    for (auto dir : { Direction::Up, Direction::Down, Direction::Left, Direction::Right }) {
        m_directionStates[dir] = false;
    }
}

void Controller::Update() {
    m_input->Update();
    // Met à jour l'état des directions en se basant sur l'état des touches
    m_directionStates[Direction::Up] = m_input->KeyIsPressed(VK_UP);
    m_directionStates[Direction::Down] = m_input->KeyIsPressed(VK_DOWN);
    m_directionStates[Direction::Left] = m_input->KeyIsPressed(VK_LEFT);
    m_directionStates[Direction::Right] = m_input->KeyIsPressed(VK_RIGHT);
}

bool Controller::IsMoving(Direction dir) const {
    auto it = m_directionStates.find(dir);
    if (it != m_directionStates.end()) {
        return it->second;
    }
    return false;
}