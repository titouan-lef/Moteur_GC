#pragma once
class IdleState : public State {
public:
    void OnEnter(Enemy* enemy) override;

    void OnUpdate(Enemy* enemy) override;

    void OnExit(Enemy* enemy) override;
};