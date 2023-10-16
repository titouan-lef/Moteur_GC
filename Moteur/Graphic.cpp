#include "Graphic.h"

void Graphic::InitCmdQueue()
{
	// Créer une structure de description de la file de commandes
	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT; // Type de file de commandes (DIRECT, COMPUTE, ou COPY)
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE; // Drapeaux de la file de commandes (peut être modifié selon vos besoins)

	// Créer la file de commandes
	pDevice->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&pCmdQueue));
}

void Graphic::InitCmdList()
{
	ID3D12CommandAllocator* pCmdAllocator = nullptr;
	pDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&pCmdAllocator));

	// Créez un objet ID3D12GraphicsCommandList pour commencer à enregistrer des commandes.
	pDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, pCmdAllocator, nullptr, IID_PPV_ARGS(&pCmdList));

	pCmdList->OMSetRenderTargets(1, &destDescriptor, FALSE, nullptr);
}

void Graphic::CreateSwapChain(HWND hWnd)
{
	InitCmdQueue();

	DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
	swapChainDesc.BufferCount = 2; // Nombre de tampons de la chaîne de permutation
	swapChainDesc.Width = 0;// Largeur de la fenêtre
	swapChainDesc.Height = 0;// Hauteur de la fenêtre
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // Format de la surface de la fenêtre
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; // Mode de permutation
	swapChainDesc.SampleDesc.Count = 1; // Nombre d'échantillons par pixel
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; // Drapeaux

	CreateDXGIFactory1(IID_PPV_ARGS(&pFactory));

	pFactory->CreateSwapChainForHwnd(
		pCmdQueue,
		hWnd,
		&swapChainDesc,
		nullptr,
		nullptr,
		&pSwapChain
	);
}

void Graphic::CreateTargetRenderView()
{
	ID3D12Resource* pBackBuffer = nullptr;
	pSwapChain->GetBuffer(0, __uuidof(ID3D12Resource), reinterpret_cast<void**>(&pBackBuffer));


	D3D12_DESCRIPTOR_HEAP_DESC descriptorHeapDesc = {};
	descriptorHeapDesc.NumDescriptors = 2; // Généralement, vous avez besoin de deux tampons de rendu pour la chaîne de swaps.
	descriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	descriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

	ID3D12DescriptorHeap* descriptorHeap = nullptr;
	pDevice->CreateDescriptorHeap(&descriptorHeapDesc, IID_PPV_ARGS(&descriptorHeap));

	destDescriptor = descriptorHeap->GetCPUDescriptorHandleForHeapStart();

	pDevice->CreateRenderTargetView(
		pBackBuffer,
		nullptr,
		destDescriptor
	);

	//pBackBuffer->Release();
}

Graphic::Graphic(HWND hWnd)
{

	D3D12CreateDevice(nullptr, featureLevel, IID_PPV_ARGS(&pDevice));

	CreateSwapChain(hWnd);
	
	CreateTargetRenderView();

	InitCmdList();
}

Graphic::~Graphic()
{
	if (pDevice != nullptr)
		pDevice->Release();

	//TO DO
}

void Graphic::EndFrame()
{
	pSwapChain->Present(1u, 0u);
}
