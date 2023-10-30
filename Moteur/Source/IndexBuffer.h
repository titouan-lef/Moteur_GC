#pragma once
#include "framwork.h"
#include "Buffer.h"

class IndexBuffer : public Buffer
{
public:
	IndexBuffer(std::vector<UINT16> m_indices);
private:
	std::vector<D3D12_INDEX_BUFFER_VIEW > m_indexBufferView = {};// tableau des indexations des vertex
};
