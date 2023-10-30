#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(std::vector<Vertex> vertices) : Buffer(vertices.size() * sizeof(Vertex), vertices.data())
{
	// Initialisation du vertex buffer view qui indique au GPU comment interpréter les données du vertex buffer
	m_vertexBufferView.push_back(D3D12_VERTEX_BUFFER_VIEW(m_buffer->GetGPUVirtualAddress(), m_bufferSize, sizeof(Vertex)));
}
