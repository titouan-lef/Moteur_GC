#include "Cube.h"
#include "Collider.h"
#include "MeshRenderer.h"
#include "Camera.h"
#include "Mesh.h"

std::vector<Vertex> Cube::m_vertices = {
    // Front face
    {{-0.5f, -0.5f, -0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}},  // 0
    {{0.5f, -0.5f, -0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}},   // 1
    {{0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}},    // 2
    {{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}},   // 3

    // Back face
    {{-0.5f, -0.5f, 0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}},   // 4
    {{0.5f, -0.5f, 0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}},    // 5
    {{0.5f, 0.5f, 0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}},     // 6
    {{-0.5f, 0.5f, 0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}},    // 7
};

std::vector<UINT16> Cube::m_indices = {
    // Front face
    0, 3, 2, 2, 1, 0,

    // Back face
    4, 7, 6, 6, 5, 4,

    // Left face
    0, 4, 7, 7, 3, 0,

    // Right face
    1, 2, 6, 6, 5, 1,

    // Top face
    3, 7, 6, 6, 2, 3,

    // Bottom face
    0, 1, 5, 5, 4, 0
};

Cube::Cube() {
    this->AddComponent<Transform>();
    this->GetComponent<Transform>()->UpdateMatrix();

    ConstantBufferData* cbd = new ConstantBufferData();
    cbd->World = this->GetComponent<Transform>()->GetMatrixTranspose();
    cbd->View = Camera::m_Instance->GetViewMatrix();
    cbd->Projection = XMMatrixTranspose(XMLoadFloat4x4(&Camera::m_projMatrix));

    this->AddComponent<MeshRenderer>()->Init(new Mesh(m_vertices, m_indices), cbd);
}

Cube::~Cube()
{
}

void Cube::Init()
{
}


void Cube::Update() {
    this->GetComponent<Transform>()->UpdateMatrix();
    this->GetComponent<Collider>()->GetCollider()->UpdateMatrix();
    ConstantBufferData cbd = ConstantBufferData();
    cbd.World = this->GetComponent<Transform>()->GetMatrixTranspose();
    cbd.View = Camera::m_Instance->GetViewMatrix();
    cbd.Projection = XMMatrixTranspose(XMLoadFloat4x4(&Camera::m_projMatrix));

    this->GetComponent<MeshRenderer>()->Update(&cbd);
}

void Cube::PostUpdate()
{
}

void Cube::Render()
{
}

void Cube::OnDestroy()
{
}