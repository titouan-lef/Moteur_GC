#include "Component.h"
#include "MeshRenderer.h"

MeshRenderer::MeshRenderer()
{
}

MeshRenderer::~MeshRenderer()
{
	delete m_constBuffer;
	delete m_mesh;
}

void MeshRenderer::Init(Mesh* mesh, ConstantBufferData* constBufferData)
{
	m_mesh = mesh;
	m_constBuffer = new ConstantBuffer(constBufferData);
}

void MeshRenderer::Update(ConstantBufferData* constBufferData)
{
	m_constBuffer->Update(constBufferData);
}
