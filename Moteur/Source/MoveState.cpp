#include "State.h"
#include "EnemyState.h"
#include "Enemy.h"
#include "MoveState.h"
void MoveState::OnEnter(Enemy* enemy)  {
    std::cout << "Enemy is now moving." << std::endl;
}

void MoveState::OnUpdate(Enemy* enemy)  {
    if (enemy->GetCurrentState() == EnemyState::Move) {
        // Logique pour l'état Move
    }
}

void MoveState::OnExit(Enemy* enemy)  {
    std::cout << "Enemy is leaving move state." << std::endl;
}