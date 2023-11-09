#pragma once
#include "Collider.h"
#include "Timer.h"
#include "Entity.h"
#include "Mesh.h"
#include "asteroid.h"


class AsteroidBizarre : public Asteroid {
public:
    Timer time;
    bool isDead = false;

    AsteroidBizarre();
    ~AsteroidBizarre();
    void CreateVertices();
    void CreateIndices();
private:
    std::vector<Vertex> m_vertices;
    std::vector<UINT16> m_indices;
};