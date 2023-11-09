#include "Cube.h"
#include "MeshRenderer.h"
#include "Collider.h"
#include "ShaderColor.h"

Cube::Cube()
{
    this->AddComponent<Transform>();
    this->AddComponent<MeshRenderer>()->Init(new Mesh(m_vertices, m_indices), ShaderColor::GetInstance());
}

Cube::~Cube()
{
}

void Cube::Update()
{
	GetComponent<Transform>()->Rotate(10*Engine::GetInstance()->Time->Peek()*0.1f, 10 * Engine::GetInstance()->Time->Peek() * 0.1f, 0);
}