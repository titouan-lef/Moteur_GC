#pragma once
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"

class MeshRenderer : public Component
{
public:
	MeshRenderer(ConstantBufferData* constBufferData);
	virtual ~MeshRenderer();


private:
	std::string m_shaderPath;

//	VertexBuffer* m_vertexBuffer;
//	IndexBuffer* m_indexBuffer;
	ConstantBuffer* m_constBuffer;

};