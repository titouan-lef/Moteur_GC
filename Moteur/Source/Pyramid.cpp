#include "MeshRenderer.h"
#include "Camera.h"
#include "Mesh.h"
#include "Collider.h"
#include "ShaderColor.h"
#include "Pyramid.h"

std::vector<Vertex> Pyramid::m_vertices = {
    // Base square
    {{-0.5f, 0.0f, -0.5f}, {1.0f, 0.0f}},  // 0 Left-back
    {{0.5f, 0.0f, -0.5f}, {1.0f, 0.0f}},   // 1 Right-back
    {{0.5f, 0.0f, 0.5f}, {1.0f, 0.0f}},    // 2 Right-front
    {{-0.5f, 0.0f, 0.5f}, {1.0f, 0.0f}},   // 3 Left-front

    // Top point
    {{0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}}     // 4 Top
};

std::vector<UINT16> Pyramid::m_indices = {
    // Base
    0, 3, 2,
    2, 1, 0,

    // Sides
    0, 1, 4,
    1, 2, 4,
    2, 3, 4,
    3, 0, 4
};
Pyramid::Pyramid() {
    this->AddComponent<Transform>();
    this->AddComponent<MeshRenderer>()->Init(new Mesh(m_vertices, m_indices), ShaderColor::GetInstance());
}

Pyramid::~Pyramid()
{
}

