#include "Cube.h"
#include "MeshRenderer.h"
#include "Camera.h"
#include "Collider.h"
#include "ShaderColor.h"

Cube::Cube() {
    this->AddComponent<Transform>();

    ConstantBufferData* cbd = new ConstantBufferData();
    cbd->World = this->GetComponent<Transform>()->GetMatrixTranspose();
    cbd->View = Camera::GetCamera()->GetTransposedView();
    cbd->Projection = Camera::GetCamera()->GetTransposedProj();

    this->AddComponent<MeshRenderer>()->Init(new Mesh(m_vertices, m_indices), new ShaderColor(cbd));
}

Cube::~Cube()
{
}