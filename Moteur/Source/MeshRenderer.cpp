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

void MeshRenderer::Init(Mesh* mesh, Shader* shader, UINT numTexture)
{
	m_mesh = mesh;
	m_shader = shader;
	m_numTexture = numTexture;
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
	if (GetOwner()->GetComponent<Transform>()->IsOnScreen())
		Engine::GetInstance()->Render(GetOwner());
}
