#pragma once
#include "Shader.h"

class ShaderColor : public Shader
{
public:
	ShaderColor(ConstantBufferData* cbd);
	virtual ~ShaderColor();

	void SetGraphicsRoot();
	ConstantBufferLight* m_constBufferLight;

private:
	static ID3D12RootSignature* s_rootSignature;

	static ID3D12RootSignature* CreateRootSignature();
};

