#pragma once
#include "framwork.h"
#include "Buffer.h"

class VertexBuffer : public Buffer
{
public:
	VertexBuffer(std::vector<Vertex> vertices);

public://TO DO : en private ?
	std::vector<D3D12_VERTEX_BUFFER_VIEW> m_vertexBufferView = {};// Tableau indiquant au GPU comment interpr�ter les donn�es de chaque vertex buffer
};

