#pragma once
#include "Shader.h"

class ShaderColor : public Shader
{
public:
	ShaderColor(ConstantBufferData* cbd);
	virtual ~ShaderColor();

	void SetGraphicsRoot();

private:
	ID3D12RootSignature* CreateRootSignature();
};

