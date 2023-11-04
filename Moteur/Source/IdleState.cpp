#include "State.h"
#include "EnemyState.h"
#include "Enemy.h"
#include "IdleState.h"

void IdleState::OnEnter(Enemy* enemy)  {
    std::cout << "Enemy is now idle." << std::endl;
}

void IdleState::OnUpdate(Enemy* enemy)  {
    if (enemy->GetCurrentState() == EnemyState::Idle) {
        // Logique pour l'état Idle
    }
}

void IdleState::OnExit(Enemy* enemy)  {
    std::cout << "Enemy is leaving idle state." << std::endl;
}