#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(std::vector<UINT16> m_indices) : Buffer(m_indices.size() * sizeof(UINT16), m_indices.data())
{
	m_nbVertex = m_indices.size();

	// Création de la vue de l'index buffer
	m_indexBufferView.BufferLocation = m_buffer->GetGPUVirtualAddress();
	m_indexBufferView.SizeInBytes = m_bufferSize;
	m_indexBufferView.Format = DXGI_FORMAT_R16_UINT;
}
