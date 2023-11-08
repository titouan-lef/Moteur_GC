#pragma once
#include "Entity.h"
#include "Mesh.h"
class Pyramid : public Entity  {
public:
    Pyramid();
    ~Pyramid();


private:
    static std::vector<Vertex> m_vertices;
    static std::vector<UINT16> m_indices;

};