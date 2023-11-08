#pragma once
#include "Shader.h"
#include "ConstantBufferSR.h"

class ShaderTexture : public Shader
{
public:
	ShaderTexture(XMMATRIX world, TextureType texture);
	virtual ~ShaderTexture();

	void SetGraphicsRoot();


	UINT imageWidth = 0;
	UINT imageHeight = 0;
	UINT imagePixelSize = 4;

	ID3D12DescriptorHeap* m_samplerHeap = nullptr;

private:
	ID3D12RootSignature* s_rootSignature;
	ID3D12RootSignature* CreateRootSignature();
};