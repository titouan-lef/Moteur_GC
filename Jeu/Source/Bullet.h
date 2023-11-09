#pragma once
#include <Timer.h>
#include "Pyramid.h"
#include "Timer.h"
#include "Entity.h"
#include "Mesh.h"

class Bullet : public Entity
{
public:
    Timer* time;
    int lifeTime = 5;

    Bullet(int x, int y);
    ~Bullet();

    virtual void Init() override;
    virtual void Update() override;
    bool isDead();

    void OnCollision(Entity* other);

private:
    std::vector<Vertex> m_vertices = {
        // Base square
        {{-0.5f, 0.0f, -0.5f}, {0.0f, 1.0f}},  // 0 Left-back
        {{0.5f, 0.0f, -0.5f}, {1.0f, 1.0f}},   // 1 Right-back
        {{0.5f, 0.0f, 0.5f}, {1.0, 0.0f}},    // 2 Right-front
        {{-0.5f, 0.0f, 0.5f}, {0.0f, 0.0f}},   // 3 Left-front

        // Top point
        {{0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}}     // 4 Top
    };

    std::vector<UINT16> m_indices = {
        // Base
        0, 3, 2,
        2, 1, 0,

        // Sides
        0, 1, 4,
        1, 2, 4,
        2, 3, 4,
        3, 0, 4
    };
};
