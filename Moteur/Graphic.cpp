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
/*
void Graphic::InitCmdList()
{
	ID3D12CommandAllocator* pCmdAllocator = nullptr;
	pDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&pCmdAllocator));

	// Créez un objet ID3D12GraphicsCommandList pour commencer à enregistrer des commandes.
	pDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, pCmdAllocator, nullptr, IID_PPV_ARGS(&pCmdList));


	


	std::vector<Vertex> vertices = {
	{ 0.0f, 1.0f, 0.0f }, // Sommet 0
	{ 1.0f, -1.0f, 0.0f }, // Sommet 1
	{ -1.0f, -1.0f, 0.0f } // Sommet 2
	};

	InitIndexBuffer(vertices);
}
*/
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
/*
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
/*
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

	pBackBuffer->Release();
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

	/***************

	pCmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // Topologie primitive
	pCmdList->OMSetRenderTargets(1, &destDescriptor, FALSE, nullptr); // Configuration des vues de rendu
	pCmdList->SetGraphicsRootSignature(pRootSignature); // Liaison de la signature racine
	pCmdList->SetPipelineState(pPipelineState);
}*/

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


/********************************************/

void Graphic::initScenegraphics(HWND hWnd)
{
	// * * * * * VERTEX- AND PIXEL-SHADER * * * * * //
	
	// Create a vertex and a pixel shader from blob-info to vertex/pixel_ptrs  
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

	// * * * * * INPUT LAYOUT * * * * * //
	// An input layout how to handle data from vertexbuffers

	// Étape 1 : Définir les éléments d'entrée
	const int nbInputLayout = 1;
	D3D12_INPUT_ELEMENT_DESC inputLayout[nbInputLayout] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	};

	// Étape 2.1 : Créer un objet D3D12_INPUT_LAYOUT_DESC
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc;
	inputLayoutDesc.pInputElementDescs = inputLayout;
	inputLayoutDesc.NumElements = nbInputLayout;

	// Étape 2.2 : Créer un objet ID3D12RootSignature
	CreateRootSignature();

	CreatePipelineStateObject();

	// Étape 3 : Utiliser l'objet D3D12_INPUT_LAYOUT_DESC lors de la création du pipeline
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc = {};
	pipelineDesc.InputLayout = inputLayoutDesc;
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

	// Étape 4 : Créez votre pipeline graphique avec ces paramètres
	pDevice->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pPipelineState));


	// * * * * * VERTEX BUFFER / INDEX BUFFER * * * * * // 
	const int nbVertex = 4;
	Vertex quad[nbVertex] =
	{
				Vertex(-0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, -1.0f, -1.0f, -1.0f),
				Vertex(-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f, 1.0f, -1.0f),
				Vertex(0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, -1.0f),
				Vertex(0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, -1.0f),
	};

	// Indices for vertex buffer
	const int nbIndice = 6;
	DWORD indices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};


	/**********************************/

	// Création des commandes queues
	InitCmdQueue();

	// Création swap chain
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


	ID3D12CommandAllocator* pCmdAllocator = nullptr;
	pDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&pCmdAllocator));

	// Créez un objet ID3D12GraphicsCommandList pour commencer à enregistrer des commandes.
	pDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, pCmdAllocator, nullptr, IID_PPV_ARGS(&pCmdList));


	// Vertex buffer desciption
	D3D12_HEAP_PROPERTIES heapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	D3D12_RESOURCE_DESC bufferDesc = CD3DX12_RESOURCE_DESC::Buffer(sizeof(Vertex) * nbVertex);

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
	D3D12_RESOURCE_DESC bufferUploadDesc = CD3DX12_RESOURCE_DESC::Buffer(sizeof(Vertex) * nbVertex);

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
	memcpy(pMappedData, indices, sizeof(indices[0]) * nbIndice);
	pUploadBuffer->Unmap(0, nullptr);

	// Copiez les données du tampon de chargement dans le tampon d'indices
	pCmdList->CopyBufferRegion(pIndexBuffer, 0, pUploadBuffer, 0, bufferUploadDesc.Width);

	// Configurez la vue d'indices
	indexBufferView.BufferLocation = pIndexBuffer->GetGPUVirtualAddress();
	indexBufferView.SizeInBytes = sizeof(UINT16) * nbIndice;
	indexBufferView.Format = DXGI_FORMAT_R16_UINT; // Format des indices

	// Configurez la vue de vertices
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView = {};
	vertexBufferView.BufferLocation = pUploadBuffer->GetGPUVirtualAddress();
	vertexBufferView.StrideInBytes = sizeof(Vertex); // Pour un sommet avec trois composantes float
	vertexBufferView.SizeInBytes = sizeof(Vertex) * nbVertex; // La taille totale du tampon de sommets



	// * * * * * Texturing * * * * * //
	D3D12_SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR; // Type de filtrage (dans cet exemple, linéaire)
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;      // Mode d'adressage U
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;      // Mode d'adressage V
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;      // Mode d'adressage W
	samplerDesc.MipLODBias = 0.0f;                         // Biais de niveau de détail Mip
	samplerDesc.MaxAnisotropy = 1;                          // Anisotropie maximale (dans cet exemple, sans anisotropie)
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_ALWAYS; // Fonction de comparaison (dans cet exemple, toujours)
	samplerDesc.BorderColor[0] = 1.0f;                    // Couleur de bordure rouge
	samplerDesc.BorderColor[1] = 1.0f;                    // Couleur de bordure verte
	samplerDesc.BorderColor[2] = 1.0f;                    // Couleur de bordure bleue
	samplerDesc.BorderColor[3] = 1.0f;                    // Couleur de bordure alpha
	samplerDesc.MinLOD = 0.0f;                           // Niveau de détail minimum
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;              // Niveau de détail maximum

	D3D12_DESCRIPTOR_HEAP_DESC descriptorHeapDesc = {};
	descriptorHeapDesc.NumDescriptors = 2; // Généralement, on a besoin de deux tampons de rendu pour la chaîne de swaps
	descriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	descriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

	ID3D12DescriptorHeap* pSamplerHeap = nullptr;
	pDevice->CreateDescriptorHeap(&descriptorHeapDesc, IID_PPV_ARGS(&pSamplerHeap));
	
	pDevice->CreateSampler(&samplerDesc, pSamplerHeap->GetCPUDescriptorHandleForHeapStart());


	// Target Render View
	ID3D12Resource* pBackBuffer = nullptr;
	pSwapChain->GetBuffer(0, __uuidof(ID3D12Resource), reinterpret_cast<void**>(&pBackBuffer));

	InitDestDescriptor();

	pDevice->CreateRenderTargetView(
		pBackBuffer,
		nullptr,
		destDescriptor
	);

	pBackBuffer->Release();


	// - - - load texturefile from file - - - //
	// TO DO




	/**********************/

	pCmdList->IASetVertexBuffers(0, 1, &vertexBufferView); // Lier le tampon de sommets


	pCmdList->SetPipelineState(pPipelineState);


	/***************/


	// - - - - - - - - - - - - - - - - - - - - //

	/*
	// * * * * * CONSTANT BUFFER CREATION * * * * * //
	// Create buffer to send to cbuffer in vertexshader

	D3D11_BUFFER_DESC cBufferDesc;
	ZeroMemory(&cBufferDesc, sizeof(D3D11_BUFFER_DESC));

	cBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	cBufferDesc.ByteWidth = sizeof(cBuffer);
	cBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cBufferDesc.CPUAccessFlags = 0;
	cBufferDesc.MiscFlags = 0;

	hr = pDevice->CreateBuffer(&cBufferDesc, NULL, &pCBuffer);
	assert(SUCCEEDED(hr));

	// - - - - -  LIGHTNING BUFFER - - - - -  //

	ZeroMemory(&cBufferDesc, sizeof(D3D11_BUFFER_DESC));

	cBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	cBufferDesc.ByteWidth = sizeof(cBufferLight);
	cBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cBufferDesc.CPUAccessFlags = 0;
	cBufferDesc.MiscFlags = 0;

	hr = pDevice->CreateBuffer(&cBufferDesc, NULL, &pCBufferLight);
	assert(SUCCEEDED(hr));*/
}





/*************************************************/


Graphic::Graphic(HWND hWnd)
{

	D3D12CreateDevice(nullptr, featureLevel, IID_PPV_ARGS(&pDevice));

	/*CreateSwapChain(hWnd);
	
	CreateTargetRenderView();

	CreateRootSignature();

	CreatePipelineStateObject();

	InitCmdList();*/

	initScenegraphics(hWnd);
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
	ClearBuffer(0.5f, 0.5f, 0.5f);

	ID3D12CommandList* ppCmdList[] = { pCmdList };
	pCmdQueue->ExecuteCommandLists(1, ppCmdList);

	pSwapChain->Present(1u, 0u);
}
