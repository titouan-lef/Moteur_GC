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
	GetComponent<Transform>()->Rotate(
        this->GetComponent<Transform>()->GetRotationSpeed().x * Engine::GetInstance()->Time->Peek(), 
        this->GetComponent<Transform>()->GetRotationSpeed().y * Engine::GetInstance()->Time->Peek(),
        this->GetComponent<Transform>()->GetRotationSpeed().z * Engine::GetInstance()->Time->Peek()
    );
}