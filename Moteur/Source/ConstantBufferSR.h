#pragma once
#include "ConstantBuffer.h"
#include "DDSTextureLoader.h"

struct Texture
{
	// Unique material name for lookup.
	std::string Name;
	std::wstring Filename;
	Microsoft::WRL::ComPtr<ID3D12Resource> Resource = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> UploadHeap = nullptr;
};

enum TextureType
{
	pierre
};

class ConstantBufferSR : public ConstantBuffer
{
public:
	ConstantBufferSR(XMMATRIX world, TextureType texture);
	virtual ~ConstantBufferSR();

	virtual void SetGraphicsRoot();

	void CreateTexture();



private:
	UINT imageWidth = 0;
	UINT imageHeight = 0;
	UINT imagePixelSize = 4;

	//std::vector<UINT8> LoadFromFile(const std::wstring& filePath);

	TextureType m_texture;

};