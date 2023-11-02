#pragma once

#include "framwork.h"
#include "DxgiInfoManager.h"

class Engine 
{
public:
	Engine();
	virtual ~Engine();
	
	static Engine* m_Instance;

	//Get the instance of the engine
	static Engine* GetInstance() { return m_Instance; }

    // Périphérique de rendu DirectX
	static ID3D12Device* Device;

	static void SetCommandList(ID3D12GraphicsCommandList* cl) { CommandList = cl; }
	void Draw(ID3D12RootSignature* rootSignature);

private:
	// Command Allocator associé au périphérique de rendu
	static ID3D12CommandAllocator* CommandAllocator;
	// Pipeline state object associé au périphérique de rendu
	static ID3D12PipelineState* PipelineStateObject;
	// Viewport associé au périphérique de rendu
	static std::vector<CD3DX12_VIEWPORT> Viewport;
	// Scissor associé au périphérique de rendu
	static std::vector<CD3DX12_RECT> Scissor;
	// Render target associé au périphérique de rendu
	static ID3D12Resource* RenderTarget;
	// BackBuffer index associé au périphérique de rendu
	static UINT BackBufferIndex;
	// Descriptor heap associé au périphérique de rendu
	static ID3D12DescriptorHeap* RTVHeap;
	// Descriptor size associé au périphérique de rendu
	static UINT RTVDescriptorSize;
	// Descriptors heap associé au périphérique de rendu
	static std::vector<ID3D12DescriptorHeap*> DescriptorsHeap;
	// Command list associée au périphérique de rendu
	static ID3D12GraphicsCommandList* CommandList;
	// Liste d'indices
	std::vector<UINT16> Indices = {};
	// tableau indiquant au GPU comment interpréter les données de chaque vertex buffer
	std::vector<D3D12_VERTEX_BUFFER_VIEW> VertexBufferView = {};
	// tableau des indexations des vertex
	std::vector<D3D12_INDEX_BUFFER_VIEW > IndexBufferView = {};


	// Gestion des erreurs
#ifndef  NDEBUG
	DxgiInfoManager infoManager = {};
#endif
};