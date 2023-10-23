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


	/****************************/


	std::vector<Vertex> vertices = {
	{ 0.0f, 1.0f, 0.0f }, // Sommet 0
	{ 1.0f, -1.0f, 0.0f }, // Sommet 1
	{ -1.0f, -1.0f, 0.0f } // Sommet 2
	};

	InitIndexBuffer(vertices);
}

void Graphic::InitDestDescriptor()
{
	D3D12_DESCRIPTOR_HEAP_DESC descriptorHeapDesc = {};
	descriptorHeapDesc.NumDescriptors = 2; // Généralement, on a besoin de deux tampons de rendu pour la chaîne de swaps
	descriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	descriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

	ID3D12DescriptorHeap* descriptorHeap = nullptr;
	pDevice->CreateDescriptorHeap(&descriptorHeapDesc, IID_PPV_ARGS(&descriptorHeap));

	destDescriptor = descriptorHeap->GetCPUDescriptorHandleForHeapStart();
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

	InitDestDescriptor();

	pDevice->CreateRenderTargetView(
		pBackBuffer,
		nullptr,
		destDescriptor
	);

	//pBackBuffer->Release();
}

void Graphic::InitIndexBuffer(std::vector<Vertex> vertices)
{

	// Définissez vos indices (assurez-vous d'adapter ceci à votre modèle)
	std::vector<UINT16> indices = { 0, 1, 2 }; // Exemple de indices pour un triangle

	// Créez un tampon de ressources pour les indices
	D3D12_HEAP_PROPERTIES heapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	D3D12_RESOURCE_DESC bufferDesc = CD3DX12_RESOURCE_DESC::Buffer(sizeof(vertices) * vertices.size());

	pDevice->CreateCommittedResource(
		&heapProperties,
		D3D12_HEAP_FLAG_NONE,
		&bufferDesc,
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(&pIndexBuffer)
	);

	// Créez un tampon de chargement pour copier les indices
	ID3D12Resource* pUploadBuffer;
	D3D12_HEAP_PROPERTIES heapUploadProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	D3D12_RESOURCE_DESC bufferUploadDesc = CD3DX12_RESOURCE_DESC::Buffer(sizeof(vertices) * vertices.size());

	pDevice->CreateCommittedResource(
		&heapUploadProperties,
		D3D12_HEAP_FLAG_NONE,
		&bufferUploadDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&pUploadBuffer)
	);

	UINT8* pMappedData = nullptr;
	pUploadBuffer->Map(0, nullptr, reinterpret_cast<void**>(&pMappedData));
	memcpy(pMappedData, indices.data(), sizeof(indices[0]) * indices.size());
	pUploadBuffer->Unmap(0, nullptr);

	// Copiez les données du tampon de chargement dans le tampon d'indices
	pCmdList->CopyBufferRegion(pIndexBuffer, 0, pUploadBuffer, 0, bufferUploadDesc.Width);

	// Configurez la vue d'indices
	indexBufferView.BufferLocation = pIndexBuffer->GetGPUVirtualAddress();
	indexBufferView.SizeInBytes = sizeof(UINT16) * indices.size();
	indexBufferView.Format = DXGI_FORMAT_R16_UINT; // Format des indices




	D3D12_VERTEX_BUFFER_VIEW vertexBufferView = {};
	vertexBufferView.BufferLocation = pUploadBuffer->GetGPUVirtualAddress();
	vertexBufferView.StrideInBytes = sizeof(Vertex); // Pour un sommet avec trois composantes float
	vertexBufferView.SizeInBytes = sizeof(vertices) * vertices.size(); // La taille totale du tampon de sommets

	pCmdList->IASetVertexBuffers(0, 1, &vertexBufferView); // Lier le tampon de sommets
	pCmdList->IASetIndexBuffer(&indexBufferView); // Lier le tampon d'indices

	/***************/

	pCmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // Topologie primitive
	pCmdList->OMSetRenderTargets(1, &destDescriptor, FALSE, nullptr); // Configuration des vues de rendu
	pCmdList->SetGraphicsRootSignature(pRootSignature); // Liaison de la signature racine
	pCmdList->SetPipelineState(pPipelineState);
}

void Graphic::CreateRootSignature()
{
	// Créer la signature racine (Root Signature)
	CD3DX12_ROOT_PARAMETER rootParameters[1];
	CD3DX12_DESCRIPTOR_RANGE descriptorRanges[1];
	descriptorRanges[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0); // Une plage pour une vue de tampon de constantes (CBV)

	rootParameters[0].InitAsDescriptorTable(1, &descriptorRanges[0], D3D12_SHADER_VISIBILITY_VERTEX);

	CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init(1, rootParameters, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ID3DBlob* pSignature;
	ID3DBlob* pError;
	D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &pSignature, &pError);

	pDevice->CreateRootSignature(0, pSignature->GetBufferPointer(), pSignature->GetBufferSize(), IID_PPV_ARGS(&pRootSignature));

}

void Graphic::CreatePipelineStateObject()
{
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc = {};

	std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayoutDesc =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	};

	FILE* vertexShaderFile = nullptr;
	fopen_s(&vertexShaderFile, "VertexShader.cso", "rb");
	fseek(vertexShaderFile, 0, SEEK_END);
	long vertexShaderSize = ftell(vertexShaderFile);
	fseek(vertexShaderFile, 0, SEEK_SET);
	byte* pVertexShaderData = new byte[vertexShaderSize];
	fread(pVertexShaderData, 1, vertexShaderSize, vertexShaderFile);
	fclose(vertexShaderFile);

	D3D12_SHADER_BYTECODE vertexShaderBytecode = {};
	vertexShaderBytecode.pShaderBytecode = pVertexShaderData; // Pointeur vers les données du shader (bytecode)
	vertexShaderBytecode.BytecodeLength = vertexShaderSize;  // Taille du bytecode en octets


	FILE* pixelShaderFile = nullptr;
	fopen_s(&pixelShaderFile, "PixelShader.cso", "rb");
	fseek(pixelShaderFile, 0, SEEK_END);
	long pixelShaderSize = ftell(pixelShaderFile);
	fseek(pixelShaderFile, 0, SEEK_SET);
	byte* pPixelShaderData = new byte[pixelShaderSize];
	fread(pPixelShaderData, 1, pixelShaderSize, pixelShaderFile);
	fclose(pixelShaderFile);

	D3D12_SHADER_BYTECODE pixelShaderBytecode = {};
	pixelShaderBytecode.pShaderBytecode = pPixelShaderData; // Pointeur vers les données du shader (bytecode)
	pixelShaderBytecode.BytecodeLength = pixelShaderSize;  // Taille du bytecode en octets


	pipelineDesc.InputLayout = { inputLayoutDesc.data(), (unsigned int)inputLayoutDesc.size() };
	pipelineDesc.pRootSignature = pRootSignature;
	pipelineDesc.VS = vertexShaderBytecode;
	pipelineDesc.PS = pixelShaderBytecode;
	pipelineDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	pipelineDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	pipelineDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	pipelineDesc.SampleMask = UINT_MAX;
	pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	pipelineDesc.NumRenderTargets = 1;
	pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	pipelineDesc.SampleDesc.Count = 1;

	pDevice->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pPipelineState));

}



Graphic::Graphic(HWND hWnd)
{

	D3D12CreateDevice(nullptr, featureLevel, IID_PPV_ARGS(&pDevice));

	CreateSwapChain(hWnd);
	
	CreateTargetRenderView();

	CreateRootSignature();

	CreatePipelineStateObject();

	InitCmdList();
}

Graphic::~Graphic()
{
	if (pDevice != nullptr)
		pDevice->Release();

	if (pSwapChain != nullptr)
		pSwapChain->Release();

	if (pFactory != nullptr)
		pFactory->Release();

	if (pCmdQueue != nullptr)
		pCmdQueue->Release();

	if (pCmdList != nullptr)
		pCmdList->Release();
}

void Graphic::EndFrame()
{
	pSwapChain->Present(1u, 0u);
}
