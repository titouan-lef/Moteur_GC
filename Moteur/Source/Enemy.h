

class Enemy {

private:
    State* state;
    EnemyState currentState;

public:
    Enemy() : state(nullptr), currentState(EnemyState::Idle) {}

    void SetState(State* newState, EnemyState newStateEnum);

    void Update();

    EnemyState GetCurrentState() const;
    
};