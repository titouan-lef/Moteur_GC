#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<UINT16> indices)
{
	m_vertexBuffer = new VertexBuffer(vertices);
	m_indexBuffer = new IndexBuffer(indices);
}

Mesh::~Mesh()
{
	delete m_indexBuffer;
	delete m_vertexBuffer;
}
