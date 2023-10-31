#pragma once
#include "Buffer.h"

struct Vertex
{
	DirectX::XMFLOAT3 m_position;
	DirectX::XMFLOAT4 m_color;
};

class VertexBuffer : public Buffer
{
public:
	VertexBuffer(std::vector<Vertex> vertices);

public://TO DO : en private ?
	std::vector<D3D12_VERTEX_BUFFER_VIEW> m_vertexBufferView = {};// Tableau indiquant au GPU comment interpréter les données de chaque vertex buffer
};

