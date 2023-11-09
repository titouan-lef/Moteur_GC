#pragma once
#include "Mesh.h"
#include "Shader.h"
#include "ConstantBuffer.h"

class Component;

class MeshRenderer : public Component
{
public:
	MeshRenderer();
	virtual ~MeshRenderer();

	void Init(Mesh* mesh, Shader* shader, UINT numTexture = -1);
	virtual void Update() override;
	virtual void Render() override;

public://TO DO : a mettre en private ?
	Mesh* m_mesh = nullptr;
	Shader* m_shader = nullptr;
	ConstantBuffer* m_constBuffer = nullptr;
	UINT m_numTexture = -1;
};