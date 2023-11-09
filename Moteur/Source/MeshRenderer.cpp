#include "Component.h"
#include "Collider.h"
#include "Camera.h"
#include "MeshRenderer.h"

MeshRenderer::MeshRenderer()
{
}

MeshRenderer::~MeshRenderer()
{
}

void MeshRenderer::Init(Mesh* mesh, Shader* shader)
{
	m_mesh = mesh;
	m_shader = shader;

	XMMATRIX world = GetOwner()->GetComponent<Transform>()->GetMatrixTranspose();
	m_constBuffer = new ConstantBuffer(world);
}

void MeshRenderer::Update()
{
    GetOwner()->GetComponent<Transform>()->UpdateMatrix();

    XMMATRIX world = GetOwner()->GetComponent<Transform>()->GetMatrixTranspose();
    GetOwner()->GetComponent<MeshRenderer>()->m_constBuffer->UpdateBuffer(world);
}

void MeshRenderer::Render()
{
	Engine::GetInstance()->Render(GetOwner());
}
