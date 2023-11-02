#pragma once
#include "Shader.h"
#include "UploadBuffer.h"

enum Texture
{
	pierre
};

class ShaderTexture : public Shader
{
public:
	ShaderTexture(Texture texture);
	virtual ~ShaderTexture();

	void CreateTexture(ID3D12GraphicsCommandList* m_commandList);

	UINT imageWidth = 0;
	UINT imageHeight = 0;
	UINT imagePixelSize = 4;


	UploadBuffer* m_uploadBuffer = nullptr;// TO DO : en private ?

private:
	static ID3D12RootSignature* m_rootSignature;
	ID3D12Resource* m_texture = nullptr;
	std::wstring m_texturePath = L"";

	static ID3D12RootSignature* CreateRootSignature();
	std::vector<UINT8> GenerateTextureData(UINT width, UINT height, UINT pixelSize);
	std::vector<UINT8> LoadFromFile();
};

