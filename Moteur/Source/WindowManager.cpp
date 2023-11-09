#include "MyException.h"
#include "Engine.h"
#include "WindowManager.h"
#include "Shader.h"
#include "Collider.h"


WindowManager::WindowManager(UINT width, UINT height, HWND hWnd)
{
    m_viewport = CD3DX12_VIEWPORT(0.0f, 0.0f, static_cast<float>(width), static_cast<float>(height));
    m_scissorRect = CD3DX12_RECT(0, 0, static_cast<LONG>(width), static_cast<LONG>(height));
    //m_entityTimer = new Timer();

    LoadPipeline(width, height, hWnd);
    LoadAssets();
}

WindowManager::~WindowManager()
{
}

void WindowManager::LoadPipeline(UINT width, UINT height, HWND hWnd)
{
    #if defined(_DEBUG)
    SetupDebugLayer();
    #endif

    CreateCommandQueue();
    CreateSwapChain(hWnd, width, height);
    CreateDescriptorHeaps();
    CreateFrameResources();
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

void WindowManager::CreateCommandQueue()
{
    D3D12_COMMAND_QUEUE_DESC queueDesc = {};
    queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    GFX_THROW_INFO_ONLY(Engine::GetInstance()->Device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_commandQueue)));
}

void WindowManager::CreateSwapChain(HWND hWnd, UINT width, UINT height)
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
    GFX_THROW_INFO_ONLY(Engine::GetInstance()->Factory->CreateSwapChainForHwnd(m_commandQueue, hWnd, &swapChainDesc, nullptr, nullptr, &swapChain));

    GFX_THROW_INFO_ONLY(Engine::GetInstance()->Factory->MakeWindowAssociation(hWnd, DXGI_MWA_NO_ALT_ENTER));

    m_swapChain = (IDXGISwapChain3*)swapChain;
    m_backBufferIndex = m_swapChain->GetCurrentBackBufferIndex();// Indique sur quel back buffer on va pouvoir travailler (ici on en a un seul donc m_backBufferIndex sera toujours égal à 0)
}

void WindowManager::CreateDescriptorHeaps()
{
    // Render Target
    D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
    rtvHeapDesc.NumDescriptors = FrameCount;
    rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    GFX_THROW_INFO_ONLY(Engine::GetInstance()->Device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&m_rtvHeap)));
    m_rtvDescriptorSize = Engine::GetInstance()->Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);// Récupération de la taille d'un descripteur

    // Shader Ressource
    D3D12_DESCRIPTOR_HEAP_DESC cbvSrvUavHeapDesc = {};
    cbvSrvUavHeapDesc.NumDescriptors = 100;
    cbvSrvUavHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    cbvSrvUavHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    GFX_THROW_INFO_ONLY(Engine::GetInstance()->Device->CreateDescriptorHeap(&cbvSrvUavHeapDesc, IID_PPV_ARGS(&m_cbvSrvUavHeap)));
    m_cbvSrvUavDescriptorSize = Engine::GetInstance()->Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);// Récupération de la taille d'un descripteur
}


void WindowManager::CreateFrameResources()
{
    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart());// Récupération de l'emplacement prévu pour la "surface de dessin" (= render target) 0

    for (UINT n = 0; n < FrameCount; n++)
    {
        GFX_THROW_INFO_ONLY(m_swapChain->GetBuffer(n, IID_PPV_ARGS(&m_renderTargets[n])));// Récupération de la "surface de dessin" n
        Engine::GetInstance()->Device->CreateRenderTargetView(m_renderTargets[n], nullptr, rtvHandle);// Création de la "surface de dessin" n dans l'emplacement prévu
        rtvHandle.Offset(1, m_rtvDescriptorSize);// Récupération de l'emplacement prévu pour la "surface de dessin" n+1
    }
}
#pragma endregion

void WindowManager::LoadAssets()
{
    CreateSyncObj();
    LoadTextures();
}

#pragma region LoadAssetsFunction

void WindowManager::CreateSyncObj()
{
    GFX_THROW_INFO_ONLY(Engine::GetInstance()->Device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence)));// Initialisation de m_fence
}

void WindowManager::LoadTextures()
{
    ResetCmdList();

    Texture* pTexture = new Texture();
    pTexture->CreateTexture((UINT)m_listTexure.size(), L"pierre", m_cbvSrvUavHeap, m_cbvSrvUavDescriptorSize);
    m_listTexure.push_back(pTexture);

    ExecuteCmdList();
    ResetCmdList();

    pTexture = new Texture();
    pTexture->CreateTexture((UINT)m_listTexure.size(), L"sol", m_cbvSrvUavHeap, m_cbvSrvUavDescriptorSize);
    m_listTexure.push_back(pTexture);

    ExecuteCmdList();
    pTexture->CreateShaderResourceView();

    for (int i = 0; i < m_listTexure.size(); ++i)
        m_listTexure[i]->CreateShaderResourceView(m_cbvSrvUavHeap, m_cbvSrvUavDescriptorSize);

    Engine::GetInstance()->SetListTexture(m_listTexure);
}
#pragma endregion

void WindowManager::PreRender()
{
    // Réinitialisaion pour enregistrer de nouvelles commandes pour la frame actuelle
    ResetCmdList();

    // Paramètre l'affichage pour fonctionner avec une liste de triangle
    Engine::GetInstance()->CmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    /* AJOUT DES COMMANDES */

    // Ajout des différentes fenêtres et de leur zone de rendu
    Engine::GetInstance()->CmdList->RSSetViewports(1, &m_viewport);          // Ajout des fenêtres (ici 1 seule)
    Engine::GetInstance()->CmdList->RSSetScissorRects(1, &m_scissorRect); // Ajout des zones de rendu (ici 1 seule)

    transition = CD3DX12_RESOURCE_BARRIER::Transition(m_renderTargets[m_backBufferIndex], D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);

    // Ajout des "surfaces de dessin" prêtes à être utilisées (ici 1 seule)
    Engine::GetInstance()->CmdList->ResourceBarrier(1, &transition);

    // Ajout des "surfaces de dessin" au back buffer (ici 1 seule)
    CD3DX12_CPU_DESCRIPTOR_HANDLE renderTarget = CD3DX12_CPU_DESCRIPTOR_HANDLE(m_rtvHeap->GetCPUDescriptorHandleForHeapStart(), m_backBufferIndex, m_rtvDescriptorSize);
    Engine::GetInstance()->CmdList->OMSetRenderTargets(1, &renderTarget, FALSE, nullptr);

    // Ajout de clearColor au premier plan pour effacer l'arrière plan par réécriture
    Engine::GetInstance()->CmdList->ClearRenderTargetView(renderTarget, m_clearColor, 1, &m_scissorRect);// Ajout de clearColor aux "surfaces de dessin" (ici 1 seule)

    ID3D12DescriptorHeap* descriptorHeaps[] = { m_cbvSrvUavHeap };
    Engine::GetInstance()->CmdList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);
}

void WindowManager::PostRender()
{
    // Indique au back buffer les "surfaces de dessin" à ne plus utiliser
    transition = CD3DX12_RESOURCE_BARRIER::Transition(m_renderTargets[m_backBufferIndex], D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
    Engine::GetInstance()->CmdList->ResourceBarrier(1, &transition);

    ExecuteCmdList();

    // Affichage de la frame.
    GFX_THROW_INFO_ONLY(m_swapChain->Present(1, 0));

    m_backBufferIndex = m_swapChain->GetCurrentBackBufferIndex();// Indique quel est le back buffer actuel (l'indice varie ici de 0 à 1 car on utilise 2 buffers : le back et front buffer)
}

void WindowManager::ResetCmdList()
{
    GFX_THROW_INFO_ONLY(Engine::GetInstance()->CmdAllocator->Reset());
    GFX_THROW_INFO_ONLY(Engine::GetInstance()->CmdList->Reset(Engine::GetInstance()->CmdAllocator, nullptr));
}

void WindowManager::ExecuteCmdList()
{
    // Indique que l'enregistrement des commandes est terminé et que le GPU peut les utiliser pour le rendu
    GFX_THROW_INFO_ONLY(Engine::GetInstance()->CmdList->Close());

    // Exécution des commandes (ici nous n'utilisons qu'une seul liste de comande)
    ID3D12CommandList* ppCommandLists[1] = { Engine::GetInstance()->CmdList };
    m_commandQueue->ExecuteCommandLists(1, ppCommandLists);

    WaitForPreviousFrame();
}

void WindowManager::WaitForPreviousFrame()
{
    m_fenceId++;// On passe à la prochaine frame

    // Indique que les commandes dans le file doivent être terminées avant de continuer
    GFX_THROW_INFO_ONLY(m_commandQueue->Signal(m_fence, m_fenceId));

    if (m_fence->GetCompletedValue() < m_fenceId)// Si la frame précédente n'a pas fini d'être traité
    {
        HANDLE fenceEvent = CreateEventEx(nullptr, FALSE, FALSE, EVENT_ALL_ACCESS);// Signale la fin du rendu d'une frame
        
        GFX_THROW_INFO_ONLY(m_fence->SetEventOnCompletion(m_fenceId, fenceEvent));// Créer un évènement qui indique si la frame précédente est traité
        WaitForSingleObject(fenceEvent, INFINITE);// Tant que la frame précédente n'est pas traité, le programme est suspendu
        CloseHandle(fenceEvent);// Supprime l'évènement de fin de rendu d'une frame
    }
} 