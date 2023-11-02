#include "framwork.h"
#include "WindowManager.h"
#include "MyException.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "Engine.h"
#include "Camera.h"// TO DO : A supprimer

#include <wrl/client.h>
#include <wincodec.h>
using Microsoft::WRL::ComPtr;

//TO DO A SUPPR
Shaders* s1 = new Shaders();

WindowManager::WindowManager(UINT width, UINT height)
{
    m_viewport = CD3DX12_VIEWPORT(0.0f, 0.0f, static_cast<float>(width), static_cast<float>(height));
    m_scissorRect = CD3DX12_RECT(0, 0, static_cast<LONG>(width), static_cast<LONG>(height));
}

WindowManager::~WindowManager()
{
}

void WindowManager::OnInit(UINT width, UINT height, HWND hWnd)
{
    LoadPipeline(width, height, hWnd);
    LoadAssets();

    //TO DO : A supprimer
    Camera::m_Instance = new Camera();
    r1 = new MyRectangle();
    r2 = new MyRectangle();
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
    ID3D12Device* tmp;
    GFX_THROW_INFO_ONLY(D3D12CreateDevice(GetHardwareAdapter(factory), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&tmp)));
    Engine::Device = tmp;
}

void WindowManager::CreateCommandQueue()
{
    D3D12_COMMAND_QUEUE_DESC queueDesc = {};
    queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    GFX_THROW_INFO_ONLY(Engine::Device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_commandQueue)));
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
    GFX_THROW_INFO_ONLY(Engine::Device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&m_rtvHeap)));
    m_rtvDescriptorSize = Engine::Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);// Récupération de la taille d'un descripteur

    s1->CreateHeap();
}


void WindowManager::CreateFrameResources()
{
    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart());// Récupération de l'emplacement prévu pour la "surface de dessin" (= render target) 0

    for (UINT n = 0; n < FrameCount; n++)
    {
        GFX_THROW_INFO_ONLY(m_swapChain->GetBuffer(n, IID_PPV_ARGS(&m_renderTargets[n])));// Récupération de la "surface de dessin" n
        Engine::Device->CreateRenderTargetView(m_renderTargets[n], nullptr, rtvHandle);// Création de la "surface de dessin" n dans l'emplacement prévu
        rtvHandle.Offset(1, m_rtvDescriptorSize);// Récupération de l'emplacement prévu pour la "surface de dessin" n+1
    }
}

void WindowManager::CreateCommandAllocator()
{
    GFX_THROW_INFO_ONLY(Engine::Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_commandAllocator)));
}
#pragma endregion

void WindowManager::LoadAssets()
{
    s1->CreateSignature();
    s1->PipelineStateColor(&m_pipelineState, L"source/shadersColor.hlsl");
    CreateCommandList();
    //s1->CreateTexture(m_commandList, L"source/pierre.jfif");
    CreateSyncObj();
}

#pragma region LoadAssetsFunction

void WindowManager::CreateCommandList()
{
    GFX_THROW_INFO_ONLY(Engine::Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_commandAllocator, m_pipelineState, IID_PPV_ARGS(&m_commandList)));
    GFX_THROW_INFO_ONLY(m_commandList->Close());// Indique que l'enregistrement des commandes est terminé et que le GPU peut les utiliser pour le rendu
}

void WindowManager::CreateSyncObj()
{
    GFX_THROW_INFO_ONLY(Engine::Device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence)));// Initialisation de m_fence
}
#pragma endregion

void WindowManager::OnUpdate()
{
    r2->GetComponent<Transform>()->MoveByVector({ 0.001f, 0, 0 });
    r2->GetComponent<Transform>()->RotateYaw(45);
    r2->Update();
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

    // Paramètre l'affichage pour fonctionner avec une liste de triangle
    m_commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    /* AJOUT DES COMMANDES */

    // Ajout de la Root Signature
    m_commandList->SetGraphicsRootSignature(s1->m_rootSignature);

    //Texture
    s1->SetHeap(m_commandList);

    // Ajout de la pipeline de rendu
    m_commandList->SetPipelineState(m_pipelineState);

    // Ajout des différentes fenêtres et de leur zone de rendu
    m_commandList->RSSetViewports(1, &m_viewport);          // Ajout des fenêtres (ici 1 seule)
    m_commandList->RSSetScissorRects(1, &m_scissorRect); // Ajout des zones de rendu (ici 1 seule)

    // Indique que m_renderTargets[m_backBufferIndex] est prête à être utilisée comme "surfaces de dessin"
    CD3DX12_RESOURCE_BARRIER transition = CD3DX12_RESOURCE_BARRIER::Transition(m_renderTargets[m_backBufferIndex], D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
    
    // Ajout des "surfaces de dessin" prêtes à être utilisées (ici 1 seule)
    m_commandList->ResourceBarrier(1, &transition);

    // Ajout des "surfaces de dessin" au back buffer (ici 1 seule)
    CD3DX12_CPU_DESCRIPTOR_HANDLE renderTarget = CD3DX12_CPU_DESCRIPTOR_HANDLE(m_rtvHeap->GetCPUDescriptorHandleForHeapStart(), m_backBufferIndex, m_rtvDescriptorSize);
    m_commandList->OMSetRenderTargets(1, &renderTarget, FALSE, nullptr);

    // Ajout de clearColor au premier plan pour effacer l'arrière plan par réécriture
    m_commandList->ClearRenderTargetView(renderTarget, m_clearColor, 1, &m_scissorRect);// Ajout de clearColor aux "surfaces de dessin" (ici 1 seule)

    // Ajout de l'affichage
    /*
    * TO DO :
    * créer un vertex buffer par forme
    * avoir une liste static dans la classe d'un objet pour avoir la matrice World de tous les objets dans une liste et appliquer la bonne matrice à la bonne instance via SV_InstanceID ?
    * mettre en place un systeme d'update des matrice World pour chaque objet
    */
    MeshRenderer* mr = r1->GetComponent<MeshRenderer>();
    ConstantBuffer* cb[] = {
        //r1->GetComponent<MeshRenderer>()->m_constBuffer,
        r2->GetComponent<MeshRenderer>()->m_constBuffer
    };

    const UINT nbInstance = 1;// Nombre d'instance (= forme du vertex buffer) à dessiner
    for (int i = 0; i < _countof(cb); ++i)
    {
        m_commandList->SetDescriptorHeaps(1, &cb[i]->m_descriptorHeaps);// Défini les descripteurs que la liste de commandes peut potentiellement utiliser (ici on utilise qu'un)
        // si plusieurs descripteur, rappeler SetGraphicsRootDescriptorTable en augmentant de 1 le premier paramètre à chaque fois
        m_commandList->SetGraphicsRootDescriptorTable(0, cb[i]->m_descriptorHeaps->GetGPUDescriptorHandleForHeapStart());// Ajout des descripteurs dont le shader a besoin pour accéder à différentes ressources
        m_commandList->IASetVertexBuffers(0, 1, &mr->m_mesh->m_vertexBuffer->m_vertexBufferView);// Ajout des vertex buffer (ici 1 seul)
        m_commandList->IASetIndexBuffer(&mr->m_mesh->m_indexBuffer->m_indexBufferView);// Ajout des index buffer (ici 1 seul)
        m_commandList->DrawIndexedInstanced(mr->m_mesh->m_indexBuffer->m_nbVertex, nbInstance, 0, 0, 0);// Affichage
    }


    // Indique au back buffer les "surfaces de dessin" à ne plus utiliser
    transition = CD3DX12_RESOURCE_BARRIER::Transition(m_renderTargets[m_backBufferIndex], D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
    m_commandList->ResourceBarrier(1, &transition);

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