#pragma once
#include<iostream>

class Enemy;



class State {
public:
    virtual ~State() = default;
    virtual void OnEnter(Enemy* enemy) = 0;
    virtual void OnUpdate(Enemy* enemy) = 0;
    virtual void OnExit(Enemy* enemy) = 0;
};