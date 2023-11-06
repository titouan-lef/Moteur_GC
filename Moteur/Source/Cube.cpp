#include "Cube.h"
#include "MeshRenderer.h"
#include "Camera.h"
#include "Collider.h"
#include "ShaderColor.h"

Cube::Cube() {
    this->AddComponent<Transform>();
    this->GetComponent<Transform>()->UpdateMatrix();

    // To DO : a supprimer
    Camera* camera = new Camera();

    ConstantBufferData* cbd = new ConstantBufferData();
    cbd->World = this->GetComponent<Transform>()->GetMatrixTranspose();
    cbd->View = camera->GetViewMatrix();
    cbd->Projection = XMMatrixTranspose(XMLoadFloat4x4(&camera->m_projMatrix));

    this->AddComponent<MeshRenderer>()->Init(new Mesh(m_vertices, m_indices), new ShaderColor(cbd));
}

Cube::~Cube()
{
}

//void Cube::Init()
//{
//}


void Cube::Update() {
    this->GetComponent<Transform>()->UpdateMatrix();
    this->GetComponent<Collider>()->GetCollider()->UpdateMatrix();

    // To DO : a supprimer
    Camera* camera = new Camera();

    ConstantBufferData* cbd = new ConstantBufferData();
    cbd->World = this->GetComponent<Transform>()->GetMatrixTranspose();
    cbd->View = camera->GetViewMatrix();
    cbd->Projection = camera->GetComponent<Transform>()->GetMatrixTranspose();

    this->GetComponent<MeshRenderer>()->m_shader->m_constBuffer->UpdateBuffer(cbd);
}

//void Cube::PostUpdate()
//{
//}
//
//void Cube::Render()
//{
//}
//
//void Cube::OnDestroy()
//{
//}