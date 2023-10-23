#pragma once
#include <dxgi1_4.h>
#include "d3dx12.h"
#include <cstdio>
#include <vector>

// * * * For Math * * * //
#include <DirectXMath.h>

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib,"d3d12.lib")

struct Vertex
{
	Vertex(float x, float y, float z,
		float colRed, float colGreen, float colBlue, float colAlpha,
		float u, float v,
		float nx, float ny, float nz)
		: pos(x, y, z), col(colRed, colGreen, colBlue, colAlpha), texcoord(u, v), normal(nx, ny, nz) { }

	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT4 col;
	DirectX::XMFLOAT3 normal;
	DirectX::XMFLOAT2 texcoord;
};


class Graphic
{
private:
	void InitCmdQueue();
	//void InitCmdList();
	void InitDestDescriptor();
	//void CreateSwapChain(HWND hWnd);
	//void CreateTargetRenderView();
	//void InitIndexBuffer(std::vector<Vertex> vertices);
	void CreateRootSignature();
	void CreatePipelineStateObject();


	/*********/
	void initScenegraphics(HWND hWnd);

public:
	Graphic(HWND hWnd);
	~Graphic();

	// Empêcher la copie via le constructeur et via l'assignement
	Graphic(const Graphic&) = delete;
	Graphic& operator=(const Graphic&) = delete;

	void EndFrame();

	void ClearBuffer(float r, float g, float b, float a = 1.0f) noexcept
	{
		pCmdList->SetGraphicsRootSignature(pRootSignature); // Liaison de la signature racine


		pCmdList->OMSetRenderTargets(1, &destDescriptor, FALSE, nullptr); // Configuration des vues de rendu


		const float color[] = { r, g, b, a };
		pCmdList->ClearRenderTargetView(destDescriptor, color, 0, nullptr);

		pCmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // Topologie primitive
		pCmdList->IASetIndexBuffer(&indexBufferView); // Lier le tampon d'indices

		pCmdList->DrawIndexedInstanced(3, 1, 0, 0, 0);

		pCmdList->Close();
	}

private:
	const D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_12_0;

	ID3D12Device* pDevice = nullptr;

	IDXGISwapChain1* pSwapChain = nullptr;
	IDXGIFactory4* pFactory = nullptr;
	ID3D12CommandQueue* pCmdQueue = nullptr;
	D3D12_CPU_DESCRIPTOR_HANDLE destDescriptor = {};

	ID3D12GraphicsCommandList* pCmdList = nullptr;


	//Buffer vertices
	ID3D12Resource* pIndexBuffer = nullptr;
	D3D12_INDEX_BUFFER_VIEW indexBufferView = {};

	// Shaders
	ID3D12PipelineState* pPipelineState = nullptr; // L'objet de pipeline
	ID3D12RootSignature* pRootSignature = nullptr; // La signature racine

	// Les objets shaders compilés
	ID3DBlob* pVertexShader = nullptr;
	ID3DBlob* pErrorBlob = nullptr;
	ID3DBlob* pPixelShader = nullptr;


	ID3D12Resource* pVertexBuffer = nullptr;

};