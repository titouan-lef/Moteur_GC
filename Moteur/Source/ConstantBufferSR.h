#pragma once
#include "ConstantBuffer.h"

enum Texture
{
	pierre
};

class ConstantBufferSR : public ConstantBuffer
{
public:
	ConstantBufferSR(XMMATRIX world, Texture texture);
	virtual ~ConstantBufferSR();

	virtual void SetGraphicsRoot();

	void CreateTexture(ID3D12GraphicsCommandList* m_commandList);

private:
	UINT imageWidth = 0;
	UINT imageHeight = 0;
	UINT imagePixelSize = 4;

	std::vector<UINT8> LoadFromFile(const std::wstring& filePath);

	Texture m_texture;
};