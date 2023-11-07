#include "Component.h"
#include "Collider.h"
#include "Camera.h"
#include "MeshRenderer.h"

MeshRenderer::MeshRenderer()
{
}

MeshRenderer::~MeshRenderer()
{
	delete m_shader;
	delete m_mesh;
}

void MeshRenderer::Init(Mesh* mesh, Shader* shader)
{
	m_mesh = mesh;
	m_shader = shader;
}

void MeshRenderer::Update()
{
    GetOwner()->GetComponent<Transform>()->UpdateMatrix();
    GetOwner()->GetComponent<Collider>()->GetCollider()->UpdateMatrix();

    ConstantBufferData* cbd = new ConstantBufferData();
    cbd->World = GetOwner()->GetComponent<Transform>()->GetMatrixTranspose();
    cbd->View = Camera::GetInstance()->GetTransposedView();
    cbd->Projection = Camera::GetInstance()->GetTransposedProj();

    GetOwner()->GetComponent<MeshRenderer>()->m_shader->m_constBuffer->UpdateBuffer(cbd);
}
