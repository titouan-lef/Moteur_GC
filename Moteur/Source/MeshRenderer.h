#pragma once
#include "Component.h"
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

public://TO DO : a mettre en private ?
	Mesh* m_mesh;
	ConstantBuffer* m_constBuffer = nullptr;

};