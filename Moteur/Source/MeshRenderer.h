#pragma once
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "Mesh.h"

class MeshRenderer : public Component
{
public:
	MeshRenderer();
	virtual ~MeshRenderer();

	void Init(Mesh* mesh, ConstantBufferData* constBufferData);

private:
	std::string m_shaderPath;

	Mesh* m_mesh = nullptr;
	ConstantBuffer* m_constBuffer = nullptr;

};