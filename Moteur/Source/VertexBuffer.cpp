#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(std::vector<Vertex> vertices) : Buffer(UINT (vertices.size() * sizeof(Vertex)))
{
	m_vertexBufferView.BufferLocation = m_buffer->GetGPUVirtualAddress();
	m_vertexBufferView.SizeInBytes = m_bufferSize;
	m_vertexBufferView.StrideInBytes = sizeof(Vertex);

	UpdateBuffer(vertices.data());
	vertices.clear();
}
