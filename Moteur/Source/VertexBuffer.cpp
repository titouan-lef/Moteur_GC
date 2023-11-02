#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(std::vector<Vertex> vertices) : Buffer(UINT (vertices.size() * sizeof(Vertex)), vertices.data())
{
	m_vertexBufferView.BufferLocation = m_buffer->GetGPUVirtualAddress();
	m_vertexBufferView.SizeInBytes = m_bufferSize;
	m_vertexBufferView.StrideInBytes = sizeof(Vertex);
}
