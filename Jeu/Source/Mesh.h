#pragma once
#include "VertexBuffer.h"
#include "IndexBuffer.h"

class Mesh
{
public:
	Mesh(std::vector<Vertex> vertices, std::vector<UINT16> indices);
	virtual ~Mesh();
public://TO DO : en private ?
	VertexBuffer* m_vertexBuffer = nullptr;
	IndexBuffer* m_indexBuffer = nullptr;
};

