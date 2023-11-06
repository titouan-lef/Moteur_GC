#include "State.h"
#include "EnemyState.h"
#include "Enemy.h"
#include "AttackState.h"
void AttackState::OnEnter(Enemy* enemy)  {
    std::cout << "Enemy is now attacking." << std::endl;
}

void AttackState::OnUpdate(Enemy* enemy)  {
    if (enemy->GetCurrentState() == EnemyState::Attack) {
        // Logique pour l'état Attack
    }
}

void AttackState::OnExit(Enemy* enemy)  {
    std::cout << "Enemy is leaving attack state." << std::endl;
}