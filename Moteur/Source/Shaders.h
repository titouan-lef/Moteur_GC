#pragma once
#include "framwork.h"
#include "DxgiInfoManager.h"
#include "Engine.h"

class Shaders
{
public:
	ID3D12RootSignature* m_rootSignature = nullptr;

	UINT imageWidth = 0;
	UINT imageHeight = 0;
	UINT imagePixelSize = 4;


	Shaders();
	virtual ~Shaders();

	void SetHeap(ID3D12GraphicsCommandList* m_commandList);
	void CreateHeap();
	void CreateSignature();
	void PipelineStateTexture(ID3D12PipelineState** m_pipelineState, LPCWSTR pFileName);
	void PipelineStateColor(ID3D12PipelineState** m_pipelineState, LPCWSTR pFileName);
	void CreateTexture(ID3D12GraphicsCommandList* m_commandList, const std::wstring& filePath);

private:
	std::vector<UINT8> GenerateTextureData(UINT width, UINT height, UINT pixelSize);
	std::vector<UINT8> LoadFromFile(const std::wstring& filePath);
	ID3D12DescriptorHeap* m_srvHeap = nullptr;
	ID3D12Resource* m_texture = nullptr;
};
