#include "State.h"
#include "EnemyState.h"
#include "Enemy.h"

void Enemy::SetState(State* newState, EnemyState newStateEnum) {
    if (state) {
        state->OnExit(this);
    }
    state = newState;
    currentState = newStateEnum;
    if (state) {
        state->OnEnter(this);
    }
}

void Enemy::Update() {
    if (state) {
        state->OnUpdate(this);
    }
}

EnemyState Enemy::GetCurrentState() const {
    return currentState;
}