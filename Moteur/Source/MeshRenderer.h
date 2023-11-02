#pragma once
#include "ConstantBuffer.h"
#include "Mesh.h"

struct Geometry
{
	
};

class Component;

class MeshRenderer : public Component
{
public:
	MeshRenderer();
	virtual ~MeshRenderer();

	void Init(Mesh* mesh, ConstantBufferData* constBufferData);
	void Update(ConstantBufferData* constBufferData);

	inline void SetShader(Shaders* shader) { this->shader = m_shader; }

private:
	Shaders* m_shader;

public:	//TO DO : a mettre en private ?
	inline ID3D12RootSignature* GetRS() { return shader->rootSignatureTexture; }
	inline ID3D12PipelineState* GetPSO() { return shader->psoDescTexture; }
	Mesh* m_mesh;
	ConstantBuffer* m_constBuffer = nullptr;
};