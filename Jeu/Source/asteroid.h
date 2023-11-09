#pragma once
#include "Collider.h"
#include "Timer.h"
#include "Entity.h"
#include "Mesh.h"

class Asteroid : public Entity {
public:
    Timer time;
    bool isDead = false;

    Asteroid();
    ~Asteroid();

    void Update();
    void isTouch(Collider* collider);
    bool isDeadByTime();
};