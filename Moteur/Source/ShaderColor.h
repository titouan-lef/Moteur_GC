#pragma once
#include "Shader.h"

class ShaderColor : public Shader
{
public:
	ShaderColor();
	virtual ~ShaderColor();

	void Init();

	static ShaderColor* GetInstance();













//public:
//	ShaderColor(XMMATRIX world);
//	virtual ~ShaderColor();
//
//	void SetGraphicsRoot();
//
//private:
//	ID3D12RootSignature* CreateRootSignature();
};

