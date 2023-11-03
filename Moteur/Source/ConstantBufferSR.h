#pragma once
#include "ConstantBuffer.h"

class ConstantBufferSR : public ConstantBuffer
{
public:
	ConstantBufferSR(ConstantBufferData* constBufferData);
	virtual ~ConstantBufferSR();

	void CreateTexture(ID3D12GraphicsCommandList* m_commandList);

private:
	UINT imageWidth = 0;
	UINT imageHeight = 0;
	UINT imagePixelSize = 4;

	std::vector<UINT8> LoadFromFile(const std::wstring& filePath);
};