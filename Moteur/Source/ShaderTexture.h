#pragma once
#include "Shader.h"
//#include "UploadBuffer.h"

enum Texture
{
	pierre
};

class ShaderTexture : public Shader
{
public:
	ShaderTexture(Texture texture);
	virtual ~ShaderTexture();

	void CreateTexture();


	UINT imageWidth = 0;
	UINT imageHeight = 0;
	UINT imagePixelSize = 4;

	// TO DO : en private ?
	//UploadBuffer* m_uploadBuffer = nullptr;
	ID3D12DescriptorHeap* m_srvHeap = nullptr;
	ID3D12DescriptorHeap* m_samplerHeap = nullptr;

private:
	static ID3D12RootSignature* m_rootSignature;
	ID3D12Resource* m_texture = nullptr;
	std::wstring m_texturePath = L"";


	static ID3D12RootSignature* CreateRootSignature();
	std::vector<UINT8> LoadFromFile();
	//std::vector<UINT8> GenerateTextureData(UINT width, UINT height, UINT pixelSize);


	void InitSRV();
	void InitSampler();
};

