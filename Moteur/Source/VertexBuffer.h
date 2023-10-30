#pragma once
#include "framwork.h"
#include "Buffer.h"

class VertexBuffer : public Buffer
{
public:
	VertexBuffer(std::vector<Vertex> vertices);
private:
	std::vector<D3D12_VERTEX_BUFFER_VIEW> m_vertexBufferView = {};// tableau indiquant au GPU comment interpr�ter les donn�es de chaque vertex buffer
};

