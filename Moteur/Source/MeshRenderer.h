#pragma once
#include "ConstantBuffer.h"
#include "Mesh.h"
#include "Shader.h"

class Component;

class MeshRenderer : public Component
{
public:
	MeshRenderer();
	virtual ~MeshRenderer();

	void Init(Mesh* mesh, ConstantBufferData* constBufferData, Shader* shader);
	void Update(ConstantBufferData* constBufferData);

private:
	std::string m_shaderPath;

public://TO DO : a mettre en private ?
	Mesh* m_mesh = nullptr;
	ConstantBuffer* m_constBuffer = nullptr;
	Shader* m_shader = nullptr;
};