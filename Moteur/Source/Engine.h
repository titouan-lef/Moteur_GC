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

    // P�riph�rique de rendu DirectX
	static ID3D12Device* Device;

	static void SetCommandList(ID3D12GraphicsCommandList* cl) { CommandList = cl; }
	void Draw(ID3D12RootSignature* rootSignature);

private:
	// Command Allocator associ� au p�riph�rique de rendu
	static ID3D12CommandAllocator* CommandAllocator;
	// Pipeline state object associ� au p�riph�rique de rendu
	static ID3D12PipelineState* PipelineStateObject;
	// Viewport associ� au p�riph�rique de rendu
	static std::vector<CD3DX12_VIEWPORT> Viewport;
	// Scissor associ� au p�riph�rique de rendu
	static std::vector<CD3DX12_RECT> Scissor;
	// Render target associ� au p�riph�rique de rendu
	static ID3D12Resource* RenderTarget;
	// BackBuffer index associ� au p�riph�rique de rendu
	static UINT BackBufferIndex;
	// Descriptor heap associ� au p�riph�rique de rendu
	static ID3D12DescriptorHeap* RTVHeap;
	// Descriptor size associ� au p�riph�rique de rendu
	static UINT RTVDescriptorSize;
	// Descriptors heap associ� au p�riph�rique de rendu
	static std::vector<ID3D12DescriptorHeap*> DescriptorsHeap;
	// Command list associ�e au p�riph�rique de rendu
	static ID3D12GraphicsCommandList* CommandList;
	// Liste d'indices
	std::vector<UINT16> Indices = {};
	// tableau indiquant au GPU comment interpr�ter les donn�es de chaque vertex buffer
	std::vector<D3D12_VERTEX_BUFFER_VIEW> VertexBufferView = {};
	// tableau des indexations des vertex
	std::vector<D3D12_INDEX_BUFFER_VIEW > IndexBufferView = {};


	// Gestion des erreurs
#ifndef  NDEBUG
	DxgiInfoManager infoManager = {};
#endif
};