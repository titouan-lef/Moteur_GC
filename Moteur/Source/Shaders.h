#pragma once
#include "framwork.h"
#include "DxgiInfoManager.h"

class Shaders
{
public:
	ID3D12RootSignature* m_rootSignature = nullptr;

	UINT imageWidth = 0;
	UINT imageHeight = 0;
	UINT imagePixelSize = 4;
	

	Shaders();
	virtual ~Shaders();

	void UpdateHeap(ID3D12GraphicsCommandList* m_commandList);
	void CreateHeap(ID3D12Device* m_device);
	void CreateSignature(ID3D12Device* m_device);
	void PipelineStateTexture(ID3D12Device* m_device, ID3D12PipelineState** m_pipelineState, LPCWSTR pFileName);
	void PipelineStateColor(ID3D12Device* m_device, ID3D12PipelineState** m_pipelineState, LPCWSTR pFileName);
	void CreateTexture(ID3D12Device* m_device, ID3D12GraphicsCommandList* m_commandList, const std::wstring& filePath);

private:
	std::vector<UINT8> GenerateTextureData();
	std::vector<UINT8> LoadFromFile(const std::wstring& filePath);
	ID3D12DescriptorHeap* m_srvHeap = nullptr;
	ID3D12Resource* m_texture = nullptr;
};
