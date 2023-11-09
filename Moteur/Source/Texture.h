#pragma once
#include "framwork.h"
#include "DxgiInfoManager.h"

class Texture
{
public:
	Texture();
	~Texture();
	void CreateTexture(UINT id, std::wstring fileName, ID3D12DescriptorHeap* cbvSrvUavHeap, UINT rtvDescriptorSize);
	void CreateShaderResourceView(ID3D12DescriptorHeap* cbvSrvUavHeap, UINT m_cbvSrvUavDescriptorSize);

	UINT GetTextureId() { return m_id; }
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPU() { return m_gpu; }

private:
	// Gestion des erreurs
#ifndef  NDEBUG
	DxgiInfoManager infoManager = {};
#endif

	UINT m_id = -1;
	std::wstring m_fileName = L"";
	UINT m_width = 0;
	UINT m_height = 0;
	Microsoft::WRL::ComPtr<ID3D12Resource> m_resource = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> m_uploadHeap = nullptr;
	D3D12_GPU_DESCRIPTOR_HANDLE m_gpu = {};
	D3D12_SHADER_RESOURCE_VIEW_DESC m_srvDesc = {};

	void GetSizeImg();
};