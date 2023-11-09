#include "Cube.h"
#include "MeshRenderer.h"
#include "Collider.h"
#include "ShaderTexture.h"

Cube::Cube()
{
    this->AddComponent<Transform>();
    this->AddComponent<MeshRenderer>()->Init(new Mesh(m_vertices, m_indices), ShaderTexture::GetInstance());
}

Cube::~Cube()
{
}

void Cube::Update()
{
	GetComponent<Transform>()->Rotate(10*Engine::GetInstance()->Time->Peek()*0.1f, 10 * Engine::GetInstance()->Time->Peek() * 0.1f, 0);
}