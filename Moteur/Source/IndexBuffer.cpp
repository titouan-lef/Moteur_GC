#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(std::vector<UINT16> m_indices) : Buffer(m_indices.size() * sizeof(UINT16), m_indices.data())
{
	m_nbVertex = m_indices.size();

	// Création de la vue de l'index buffer
	m_indexBufferView.push_back(D3D12_INDEX_BUFFER_VIEW(m_buffer->GetGPUVirtualAddress(), m_bufferSize, DXGI_FORMAT_R16_UINT));
}
