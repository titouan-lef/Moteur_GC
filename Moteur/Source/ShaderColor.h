#pragma once
#include "Shader.h"

class ShaderColor : public Shader
{
public:
	ShaderColor();
	virtual ~ShaderColor();

private:
	static ID3D12RootSignature* m_rootSignature;

	static ID3D12RootSignature* CreateRootSignature();
};

