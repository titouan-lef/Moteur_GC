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

    XMMATRIX world = GetOwner()->GetComponent<Transform>()->GetMatrixTranspose();
    GetOwner()->GetComponent<MeshRenderer>()->m_shader->m_constBuffer->UpdateBuffer(world);
}

void MeshRenderer::Render()
{
	if (GetOwner()->GetComponent<Transform>()->IsOnScreen())
		Engine::GetInstance()->Render(GetOwner());
}
