#pragma once
#include "Buffer.h"

class IndexBuffer : public Buffer
{
public:
	IndexBuffer(std::vector<UINT16> m_indices);

public://TO DO : n private ?
	UINT m_nbVertex = 0;
	D3D12_INDEX_BUFFER_VIEW m_indexBufferView = {};// Tableau des indexations des vertex
};
