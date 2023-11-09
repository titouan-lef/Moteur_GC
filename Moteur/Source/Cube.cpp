#include "Cube.h"
#include "MeshRenderer.h"
#include "Camera.h"
#include "Collider.h"
#include "ShaderColor.h"

Cube::Cube() {
    this->AddComponent<Transform>();

    XMMATRIX world = this->GetComponent<Transform>()->GetMatrixTranspose();
    this->AddComponent<MeshRenderer>()->Init(new Mesh(m_vertices, m_indices), new ShaderColor(world));
}

Cube::~Cube()
{
}

void Cube::Update()
{
	GetComponent<Transform>()->Rotate(0.5 * Engine::GetInstance()->Time->Peek(), 0.5* Engine::GetInstance()->Time->Peek(), 0.5 * Engine::GetInstance()->Time->Peek());
}