#pragma once
#include <dxgi1_4.h>
#include "d3dx12.h"

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib,"d3d12.lib")

class Graphic
{
private:
	void InitCmdQueue();
	void InitCmdList();
	void CreateSwapChain(HWND hWnd);
	void CreateTargetRenderView();

public:
	Graphic(HWND hWnd);
	~Graphic();

	// Empêcher la copie via le constructeur
	//Graphic(const Graphic&) = delete;
	//Graphic(const Graphic&) = default;
	// Empêcher la copie via l'assignement
	//Graphic& operator=(const Graphic&) = delete;
	//Graphic& operator=(const Graphic&) = default;

	void EndFrame();

	void ClearBuffer(float r, float g, float b) noexcept
	{
		const float color[] = { 1.0f, 0.0f, 0.0f, 1.0f };
		pCmdList->ClearRenderTargetView(destDescriptor, color, 0, nullptr);

		pCmdList->Close();
		ID3D12CommandList* ppCmdList[] = { pCmdList };
		pCmdQueue->ExecuteCommandLists(1, ppCmdList);
	}

private:
	ID3D12Device* pDevice = nullptr;

	IDXGISwapChain1* pSwapChain = nullptr;
	IDXGIFactory4* pFactory = nullptr;
	ID3D12CommandQueue* pCmdQueue = nullptr;
	D3D12_CPU_DESCRIPTOR_HANDLE destDescriptor;

	ID3D12GraphicsCommandList* pCmdList = nullptr;

	const D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_12_0;
};