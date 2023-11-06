#include "Component.h"
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
