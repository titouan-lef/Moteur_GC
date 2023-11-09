#pragma once
#include "Collider.h"
#include "Timer.h"
#include "Entity.h"
#include "Mesh.h"

class Asteroid : public Entity{
public:
    Timer time;
    bool isDead = false;

    Asteroid();
    ~Asteroid();

    virtual void Init() override;
    virtual void Update() override;

    void OnCollision(Entity* other);

    void isTouch(Collider* collider);
    bool isDeadByTime();

private:
    std::vector<Vertex> m_vertices = {
        // Front face
        {{-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f}},  // 0
        {{0.5f, -0.5f, -0.5f}, {0.5f, 1.0f}},   // 1
        {{0.5f, 0.5f, -0.5f}, {0.5f, 0.0f}},    // 2
        {{-0.5f, 0.5f, -0.5f}, {0.0f, 0.0f}},   // 3

        // Back face
        {{-0.5f, -0.5f, 0.5f}, {0.5f, 1.0f}},   // 4
        {{0.5f, -0.5f, 0.5f}, {1.0f, 1.0f}},    // 5
        {{0.5f, 0.5f, 0.5f}, {1.0f, 0.0f}},     // 6
        {{-0.5f, 0.5f, 0.5f}, {0.5f, 0.0f}},    // 7
    };

    std::vector<UINT16> m_indices = {
        // Front face
        0, 3, 2, 2, 1,0,

        // Back face
        4, 5, 6, 6, 7, 4,

        // Left face
        0, 4, 7, 7, 3, 0,

        // Right face
        1, 2, 6, 6, 5, 1,

        // Top face
        3, 7, 6, 6, 2, 3,

        // Bottom face
        0, 1, 5, 5, 4, 0
    };
};
