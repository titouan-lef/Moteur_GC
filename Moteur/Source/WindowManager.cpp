#include "WindowManager.h"
#include "MyException.h"

WindowManager::WindowManager(UINT width, UINT height)
{
    m_viewport.push_back(CD3DX12_VIEWPORT(0.0f, 0.0f, static_cast<float>(width), static_cast<float>(height)));
    m_scissorRect.push_back(CD3DX12_RECT(0, 0, static_cast<LONG>(width), static_cast<LONG>(height)));
}

WindowManager::~WindowManager()
{
    m_indices.clear();
}

void WindowManager::OnInit(UINT width, UINT height, HWND hWnd)
{
    LoadPipeline(width, height, hWnd);
    LoadAssets();
}

void WindowManager::LoadPipeline(UINT width, UINT height, HWND hWnd)
{
    #if defined(_DEBUG)
    SetupDebugLayer();
    #endif

    IDXGIFactory4* factory = CreateDXGIFactory();
    CreateD3DDevice(factory);
    CreateCommandQueue();
    CreateSwapChain(hWnd, width, height, factory);
    CreateDescriptorHeaps();
    CreateFrameResources();
    CreateCommandAllocator();
}

#pragma region LoadPipelineFunction
void WindowManager::SetupDebugLayer()
{
    ID3D12Debug* debugController;
    if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
    {
        debugController->EnableDebugLayer();
    }
}

IDXGIFactory4* WindowManager::CreateDXGIFactory()
{
    UINT dxgiFactoryFlags = 0;

    #if defined(_DEBUG)
    {
        ID3D12Debug* debugController;
        if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
        {
            debugController->EnableDebugLayer();
            dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
        }
    }
    #endif

    IDXGIFactory4* factory;
    GFX_THROW_INFO_ONLY(CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&factory)));

    return factory;
}

void WindowManager::CreateD3DDevice(IDXGIFactory4* factory)
{
    GFX_THROW_INFO_ONLY(D3D12CreateDevice(GetHardwareAdapter(factory), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_device)));
}

void WindowManager::CreateCommandQueue()
{
    D3D12_COMMAND_QUEUE_DESC queueDesc = {};
    queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    GFX_THROW_INFO_ONLY(m_device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_commandQueue)));
}

void WindowManager::CreateSwapChain(HWND hWnd, UINT width, UINT height, IDXGIFactory4* factory)
{
    DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
    swapChainDesc.BufferCount = FrameCount;
    swapChainDesc.Width = width;
    swapChainDesc.Height = height;
    swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapChainDesc.SampleDesc.Count = 1;

    IDXGISwapChain1* swapChain;
    GFX_THROW_INFO_ONLY(factory->CreateSwapChainForHwnd(m_commandQueue, hWnd, &swapChainDesc, nullptr, nullptr, &swapChain));

    GFX_THROW_INFO_ONLY(factory->MakeWindowAssociation(hWnd, DXGI_MWA_NO_ALT_ENTER));

    m_swapChain = (IDXGISwapChain3*)swapChain;
    m_backBufferIndex = m_swapChain->GetCurrentBackBufferIndex();// Indique sur quel back buffer on va pouvoir travailler (ici on en a un seul donc m_backBufferIndex sera toujours égal à 0)
}

void WindowManager::CreateDescriptorHeaps()
{
    D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
    rtvHeapDesc.NumDescriptors = FrameCount;
    rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    GFX_THROW_INFO_ONLY(m_device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&m_rtvHeap)));
    m_rtvDescriptorSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);// Récupération de la taille d'un descripteur
}

void WindowManager::CreateFrameResources()
{
    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart());// Récupération de l'emplacement prévu pour la "surface de dessin" (= render target) 0

    for (UINT n = 0; n < FrameCount; n++)
    {
        GFX_THROW_INFO_ONLY(m_swapChain->GetBuffer(n, IID_PPV_ARGS(&m_renderTargets[n])));// Récupération de la "surface de dessin" n
        m_device->CreateRenderTargetView(m_renderTargets[n], nullptr, rtvHandle);// Création de la "surface de dessin" n dans l'emplacement prévu
        rtvHandle.Offset(1, m_rtvDescriptorSize);// Récupération de l'emplacement prévu pour la "surface de dessin" n+1
    }
}

void WindowManager::CreateCommandAllocator()
{
    GFX_THROW_INFO_ONLY(m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_commandAllocator)));
}
#pragma endregion

void WindowManager::LoadAssets()
{
    CreateRootSignature();
    CreatePipelineStateObject();
    CreateCommandList();
    CreateVertexBuffer();
    CreateIndexBuffer();
    CreateConstantBuffer();
    CreateSyncObj();
}

#pragma region LoadAssetsFunction
void WindowManager::CreateRootSignature()
{
    /*
    * Création d'un descriptor table
    * cbvTable.Init(a, b, c) :
    * * b : nombre de constant buffer par objet
    * * c : regsitre du shader
    */
    CD3DX12_DESCRIPTOR_RANGE cbvTable;
    cbvTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);

    // Liste des différent Descriptor Range
    const UINT nbDescriptorRange = 1;
    CD3DX12_DESCRIPTOR_RANGE descriptorRange[nbDescriptorRange];
    descriptorRange[0] = cbvTable;

    /*
    * Tableau des paramètres de la signature racine
    * il existe 3 types de paramètres différents : root constant, root descriptor et descriptor table
    */
    const UINT nbSlot = 1;
    CD3DX12_ROOT_PARAMETER slotRootParameter[nbSlot];

    // Initialisation des paramètres de la signature racine
    slotRootParameter[0].InitAsDescriptorTable(nbDescriptorRange, descriptorRange);

    // Description de la disposition de la signature racine
    CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc(nbSlot, slotRootParameter, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

    // Transformation de la description en une structure de données qui peut être utilisée pour créer la signature racine
    ID3DBlob* serializedRootSig = nullptr;
    D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1, &serializedRootSig, nullptr);

    // Création de la signature racine
    GFX_THROW_INFO_ONLY(m_device->CreateRootSignature(0, serializedRootSig->GetBufferPointer(), serializedRootSig->GetBufferSize(), IID_PPV_ARGS(&m_rootSignature)));
}

void WindowManager::CreatePipelineStateObject()
{
    #if defined(_DEBUG) 
    UINT compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
    #else
    UINT compileFlags = 0;
    #endif

    // Récupération des shaders compilés
    ID3DBlob* vertexShader = nullptr;
    ID3DBlob* pixelShader = nullptr;
    GFX_THROW_INFO_ONLY(D3DCompileFromFile(L"Source/shaders.hlsl", nullptr, nullptr, "VSMain", "vs_5_0", compileFlags, 0, &vertexShader, nullptr));
    GFX_THROW_INFO_ONLY(D3DCompileFromFile(L"Source/shaders.hlsl", nullptr, nullptr, "PSMain", "ps_5_0", compileFlags, 0, &pixelShader, nullptr));

    // Définition du vertex input layout
    D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
    };

    // Paramétrage de la pipeline state object (PSO).
    D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
    psoDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };
    psoDesc.pRootSignature = m_rootSignature;
    psoDesc.VS = CD3DX12_SHADER_BYTECODE(vertexShader);
    psoDesc.PS = CD3DX12_SHADER_BYTECODE(pixelShader);
    psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
    psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
    psoDesc.DepthStencilState.DepthEnable = FALSE;
    psoDesc.DepthStencilState.StencilEnable = FALSE;
    psoDesc.SampleMask = UINT_MAX;
    psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    psoDesc.NumRenderTargets = 1;
    psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
    psoDesc.SampleDesc.Count = 1;

    // Création de la PSO
    GFX_THROW_INFO_ONLY(m_device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_pipelineState)));
}

void WindowManager::CreateCommandList()
{
    GFX_THROW_INFO_ONLY(m_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_commandAllocator, m_pipelineState, IID_PPV_ARGS(&m_commandList)));
    GFX_THROW_INFO_ONLY(m_commandList->Close());// Indique que l'enregistrement des commandes est terminé et que le GPU peut les utiliser pour le rendu
}

ID3D12Resource* WindowManager::CreateBuffer(UINT bufferSize, const void* src)
{
    ID3D12Resource* buffer = nullptr;
    auto tmp1 = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
    auto tmp2 = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);
    GFX_THROW_INFO_ONLY(m_device->CreateCommittedResource(
        &tmp1,
        D3D12_HEAP_FLAG_NONE,
        &tmp2,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&buffer)));

    // Copie des données dans le buffer
    BYTE* mappedData = nullptr;
    GFX_THROW_INFO_ONLY(buffer->Map(0, nullptr, reinterpret_cast<void**>(&mappedData)));
    memcpy(mappedData, src, bufferSize);
    buffer->Unmap(0, nullptr);

    return buffer;
}

void WindowManager::CreateVertexBuffer()
{
    std::vector<Vertex>vertices = {
        // Carr�
        { { -0.5f, 0.5f, 0 }, { 1.0f, 1.0f, 1.0f, 1.0f } },// Coin sup�rieur gauche
        { { 0.5f, 0.5f, 0 }, { 1.0f, 1.0f, 1.0f, 1.0f } },// Coin sup�rieur droit
        { { -0.5f, -0.5f, 0 }, { 1.0f, 1.0f, 1.0f, 1.0f } },// Coin inf�rieur gauche
        { { 0.5f, -0.5f, 0 }, { 1.0f, 1.0f, 1.0f, 1.0f } },// Coin inf�rieur droit
    };

    // Création du vertex buffer
    const UINT vertexBufferSize = (UINT) (vertices.size() * sizeof(Vertex));
    ID3D12Resource* vertexBuffer = CreateBuffer(vertexBufferSize, vertices.data());

    // Initialisation du vertex buffer view qui indique au GPU comment interpréter les données du vertex buffer
    m_vertexBufferView.push_back(D3D12_VERTEX_BUFFER_VIEW(vertexBuffer->GetGPUVirtualAddress(), vertexBufferSize, sizeof(Vertex)));
}

void WindowManager::CreateIndexBuffer()
{
    // Définition de votre tableau d'indices
    m_indices = { 0, 1, 2, 2, 1, 3 };

    // Création d'une allocation de mémoire pour l'index buffer
    const UINT indexBufferSize = (UINT)(m_indices.size() * sizeof(UINT16));
    ID3D12Resource* indexBuffer = CreateBuffer(indexBufferSize, m_indices.data());

    // Création de la vue de l'index buffer
    m_indexBufferView.push_back(D3D12_INDEX_BUFFER_VIEW(indexBuffer->GetGPUVirtualAddress(), indexBufferSize, DXGI_FORMAT_R16_UINT));
}

void WindowManager::CreateConstantBuffer()
{

    struct ConstantBufferData
    {
        DirectX::XMMATRIX World;
    };

    {
        // Création de la matrice monde
        e1 = Entity();

        ConstantBufferData* constBufferData = new ConstantBufferData();
        e1.m_Transform.SetScale(0.5f, 0.5f, 0.5f);
        e1.m_Transform.MoveByVector({ 0.5f, 0, 0.5f });
        //e.m_Transform.Rotate(0.5f, -0.25f, 0);
        e1.m_Transform.UpdateMatrix();
        constBufferData->World = e1.m_Transform.GetMatrixTranspose();
        /**************************************/

        // Création du constant buffer
        const UINT constBufferSize = (sizeof(ConstantBufferData) + 255) & ~255;
        ID3D12Resource* constBuffer = CreateBuffer(constBufferSize, constBufferData);

        // Défini l'emplacement et la taille des données du constant buffer
        D3D12_CONSTANT_BUFFER_VIEW_DESC constBufferView = {};
        constBufferView.BufferLocation = constBuffer->GetGPUVirtualAddress();// Localisation du constant buffer
        constBufferView.SizeInBytes = constBufferSize;// Taille du constant buffer

        // Propriétés du tas de descripteurs CBV_SRV_UAV (Constant Buffer View - Shader Resource Views - Unordered Access Views) permettant d'accéder à des ressources du shader
        D3D12_DESCRIPTOR_HEAP_DESC cbvSrvUavHeapDesc = {};
        cbvSrvUavHeapDesc.NumDescriptors = 1;
        cbvSrvUavHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
        cbvSrvUavHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

        // Création du tas de descripteurs CBV_SRV_UAV dont le shader a besoin pour accéder aux différentes ressources
        ID3D12DescriptorHeap* cbvSrvUavHeap = nullptr;
        m_device->CreateDescriptorHeap(&cbvSrvUavHeapDesc, IID_PPV_ARGS(&cbvSrvUavHeap));

        // Stockage du constant buffer view dans le tas
        m_device->CreateConstantBufferView(&constBufferView, cbvSrvUavHeap->GetCPUDescriptorHandleForHeapStart());

        descriptorHeaps.push_back(cbvSrvUavHeap);
    }
    {
        // Création de la matrice monde
        e2 = Entity();

        ConstantBufferData* constBufferData = new ConstantBufferData();
        e2.m_Transform.MoveByVector({ -0.5f, 0, 0.5f });
        e2.m_Transform.SetScale(0.5f, 1, 0.5f);
        e2.m_Transform.UpdateMatrix();
        constBufferData->World = e2.m_Transform.GetMatrixTranspose();
        /**************************************/

        // Création du constant buffer
        const UINT constBufferSize = (sizeof(ConstantBufferData) + 255) & ~255;
        ID3D12Resource* constBuffer = CreateBuffer(constBufferSize, constBufferData);

        // Défini l'emplacement et la taille des données du constant buffer
        D3D12_CONSTANT_BUFFER_VIEW_DESC constBufferView = {};
        constBufferView.BufferLocation = constBuffer->GetGPUVirtualAddress();// Localisation du constant buffer
        constBufferView.SizeInBytes = constBufferSize;// Taille du constant buffer

        // Propriétés du tas de descripteurs CBV_SRV_UAV (Constant Buffer View - Shader Resource Views - Unordered Access Views) permettant d'accéder à des ressources du shader
        D3D12_DESCRIPTOR_HEAP_DESC cbvSrvUavHeapDesc = {};
        cbvSrvUavHeapDesc.NumDescriptors = 1;
        cbvSrvUavHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
        cbvSrvUavHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

        // Création du tas de descripteurs CBV_SRV_UAV dont le shader a besoin pour accéder aux différentes ressources
        ID3D12DescriptorHeap* cbvSrvUavHeap = nullptr;
        m_device->CreateDescriptorHeap(&cbvSrvUavHeapDesc, IID_PPV_ARGS(&cbvSrvUavHeap));

        // Stockage du constant buffer view dans le tas
        m_device->CreateConstantBufferView(&constBufferView, cbvSrvUavHeap->GetCPUDescriptorHandleForHeapStart());

        descriptorHeaps.push_back(cbvSrvUavHeap);
    }
    
}

void WindowManager::CreateSyncObj()
{
    GFX_THROW_INFO_ONLY(m_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence)));// Initialisation de m_fence
}
#pragma endregion

void WindowManager::OnUpdate()
{
    struct ConstantBufferData
    {
        DirectX::XMMATRIX World;
    };
    m_input.Update();

    if (m_input.KeyIsPressedOnce(1) ) {

        if (e1.m_Transform.GetPosition().y + 0.01f > 1) {
    
            e1.m_Transform.SetPosition(e1.m_Transform.GetPosition().x, e1.m_Transform.GetPosition().y, e1.m_Transform.GetPosition().z);
        }
        else {
    
            e1.m_Transform.SetPosition(e1.m_Transform.GetPosition().x, e1.m_Transform.GetPosition().y + 0.01f, e1.m_Transform.GetPosition().z);
        }
    
    }

    e1.m_Transform.UpdateMatrix();

    ConstantBufferData* constBufferData = new ConstantBufferData();
    constBufferData->World = e1.m_Transform.GetMatrixTranspose();
    /**************************************/

    // Création du constant buffer
    const UINT constBufferSize = (sizeof(ConstantBufferData) + 255) & ~255;
    ID3D12Resource* constBuffer = CreateBuffer(constBufferSize, constBufferData);

    // Défini l'emplacement et la taille des données du constant buffer
    D3D12_CONSTANT_BUFFER_VIEW_DESC constBufferView = {};
    constBufferView.BufferLocation = constBuffer->GetGPUVirtualAddress();// Localisation du constant buffer
    constBufferView.SizeInBytes = constBufferSize;// Taille du constant buffer

    // Propriétés du tas de descripteurs CBV_SRV_UAV (Constant Buffer View - Shader Resource Views - Unordered Access Views) permettant d'accéder à des ressources du shader
    D3D12_DESCRIPTOR_HEAP_DESC cbvSrvUavHeapDesc = {};
    cbvSrvUavHeapDesc.NumDescriptors = 1;
    cbvSrvUavHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    cbvSrvUavHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

    // Création du tas de descripteurs CBV_SRV_UAV dont le shader a besoin pour accéder aux différentes ressources
    ID3D12DescriptorHeap* cbvSrvUavHeap = nullptr;
    m_device->CreateDescriptorHeap(&cbvSrvUavHeapDesc, IID_PPV_ARGS(&cbvSrvUavHeap));

    // Stockage du constant buffer view dans le tas
    m_device->CreateConstantBufferView(&constBufferView, cbvSrvUavHeap->GetCPUDescriptorHandleForHeapStart());

    descriptorHeaps[0] = cbvSrvUavHeap;

}

void WindowManager::OnRender()
{
    PopulateCommandList();

    // Exécution des commandes (ici nous n'utilisons qu'une seul liste de comande)
    ID3D12CommandList* ppCommandLists[] = { m_commandList };
    m_commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

    // Affichage de la frame.
    GFX_THROW_INFO_ONLY(m_swapChain->Present(1, 0));

    WaitForPreviousFrame();
}

void WindowManager::PopulateCommandList()
{
    // Réinitialisaion pour enregistrer de nouvelles commandes pour la frame actuelle
    GFX_THROW_INFO_ONLY(m_commandAllocator->Reset());
    GFX_THROW_INFO_ONLY(m_commandList->Reset(m_commandAllocator, m_pipelineState));


    /* GESTION DES BUFFER */

    // Gestion Vertex Buffer
    m_commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);// Paramètre l'affichage pour fonctionner avec une liste de triangle


    /* AJOUT DES COMMANDES */

    // Ajout de la Root Signature
    m_commandList->SetGraphicsRootSignature(m_rootSignature);// Ajout de la root signature

    // Ajout de la pipeline de rendu
    m_commandList->SetPipelineState(m_pipelineState);

    // Ajout des différentes fenêtres et de leur zone de rendu
    m_commandList->RSSetViewports((UINT)m_viewport.size(), m_viewport.data());// Ajout des fenêtres
    m_commandList->RSSetScissorRects((UINT)m_scissorRect.size(), m_scissorRect.data());// Ajout des zones de rendu

    // Ajout des "surfaces de dessin" à utiliser
    CD3DX12_RESOURCE_BARRIER transition[] = {
        CD3DX12_RESOURCE_BARRIER::Transition(m_renderTargets[m_backBufferIndex], D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET)// Indique que m_renderTargets[m_backBufferIndex] est prête à être utilisée comme "surfaces de dessin"
    };
    m_commandList->ResourceBarrier(_countof(transition), transition);// Ajout des "surfaces de dessin" prêtes à être utilisées

    // Ajout des "surfaces de dessin" au back buffer
    CD3DX12_CPU_DESCRIPTOR_HANDLE renderTarget[] = {
        CD3DX12_CPU_DESCRIPTOR_HANDLE(m_rtvHeap->GetCPUDescriptorHandleForHeapStart(), m_backBufferIndex, m_rtvDescriptorSize)
    };
    m_commandList->OMSetRenderTargets(_countof(renderTarget), renderTarget, FALSE, nullptr);

    // Ajout de clearColor au premier plan pour effacer l'arrière plan par réécriture
    const float clearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f };
    m_commandList->ClearRenderTargetView(renderTarget[0], clearColor, (UINT)m_scissorRect.size(), m_scissorRect.data());

    // Ajout de l'affichage
    const UINT nbForme = 1;// Nombre d'instance (= forme du vertex buffer) à dessiner
    /*
    * TO DO :
    * créer un vertex buffer par forme
    * avoir une liste static dans la classe d'un objet pour avoir la matrice World de tous les objets dans une liste et appliquer la bonne matrice à la bonne instance via SV_InstanceID ?
    * mettre en place un systeme d'update des matrice World pour chaque objet
    */
    for (int i = 0; i < descriptorHeaps.size(); ++i)
    {
        m_commandList->SetDescriptorHeaps(1, &descriptorHeaps[i]);// Défini les descripteurs que la liste de commandes peut potentiellement utiliser
        m_commandList->SetGraphicsRootDescriptorTable(0, descriptorHeaps[i]->GetGPUDescriptorHandleForHeapStart());// Ajout des descripteurs dont le shader a besoin pour accéder à différentes ressources (associé au constant buffer)
        m_commandList->IASetVertexBuffers(0, (UINT)m_vertexBufferView.size(), m_vertexBufferView.data());// Ajout des vertex buffer
        m_commandList->IASetIndexBuffer(&m_indexBufferView[0]);// Ajout des index buffer
        m_commandList->DrawIndexedInstanced((UINT)m_indices.size(), nbForme, 0, 0, 0);// Affichage
    }


    // Indique au back buffer les render target à ne plus utiliser
    transition[0] = CD3DX12_RESOURCE_BARRIER::Transition(m_renderTargets[m_backBufferIndex], D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);// Indique que m_renderTargets[m_backBufferIndex] ne doit plus être utilisée comme render target
    m_commandList->ResourceBarrier(_countof(transition), transition);

    // Indique que l'enregistrement des commandes est terminé et que le GPU peut les utiliser pour le rendu
    GFX_THROW_INFO_ONLY(m_commandList->Close());
}

void WindowManager::WaitForPreviousFrame()
{
    // Indique que les commandes dans le file doivent être terminées avant de continuer
    const UINT64 fence = m_fenceId;
    GFX_THROW_INFO_ONLY(m_commandQueue->Signal(m_fence, fence));

    if (m_fence->GetCompletedValue() < fence)// Si la frame précédente n'a pas fini d'être traité
    {
        HANDLE fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);// Signale la fin du rendu d'une frame

        if (fenceEvent != nullptr)// Vérifie que la création de l'événement s'est déroulée avec succès
        {
            GFX_THROW_INFO_ONLY(m_fence->SetEventOnCompletion(fence, fenceEvent));// Créer un évènement qui indique si la frame précédente est traité
            WaitForSingleObject(fenceEvent, INFINITE);// Tant que la frame précédente n'est pas traité, le programme est suspendu
            CloseHandle(fenceEvent);// Supprime l'évènement de fin de rendu d'une frame
        }
    }

    m_backBufferIndex = m_swapChain->GetCurrentBackBufferIndex();// Indique quel est le back buffer actuel (l'indice varie ici de 0 à 1 car on utilise 2 buffers : le back et front buffer)
    m_fenceId++;// On passe à la prochaine frame
}

#pragma region HardwareAdapter
bool WindowManager::IsValidAdapter(IDXGIAdapter1* adapter)
{
    DXGI_ADAPTER_DESC1 desc;
    adapter->GetDesc1(&desc);
    return !(desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) && SUCCEEDED(D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr));
}

bool WindowManager::AdapterFind(IDXGIFactory6* factory6, UINT adapterIndex, bool requestHighPerformanceAdapter, IDXGIAdapter1** pAdapter)
{
    return SUCCEEDED(factory6->EnumAdapterByGpuPreference(
        adapterIndex,
        requestHighPerformanceAdapter == true ? DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE : DXGI_GPU_PREFERENCE_UNSPECIFIED,
        IID_PPV_ARGS(pAdapter)));
}

IDXGIAdapter1* WindowManager::GetHardwareAdapter(IDXGIFactory1* pFactory, bool requestHighPerformanceAdapter)
{
    IDXGIAdapter1* adapter = nullptr;
    IDXGIFactory6* factory6 = nullptr;

    if (SUCCEEDED(pFactory->QueryInterface(IID_PPV_ARGS(&factory6))))
    {
        for (UINT adapterIndex = 0; AdapterFind(factory6, adapterIndex, requestHighPerformanceAdapter, &adapter); ++adapterIndex)
        {
            if (IsValidAdapter(adapter))
                break;
        }
    }

    if (adapter == nullptr)
    {
        for (UINT adapterIndex = 0; SUCCEEDED(pFactory->EnumAdapters1(adapterIndex, &adapter)); ++adapterIndex)
        {
            if (IsValidAdapter(adapter))
                break;
        }
    }

    return adapter;
}
#pragma endregion