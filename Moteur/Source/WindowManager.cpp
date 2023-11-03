#include "framwork.h"
#include "WindowManager.h"
#include "MyException.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "Engine.h"
#include "Camera.h"// TO DO : A supprimer
//#include "ShaderTexture.h"
#include "ShaderColor.h"

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

    CreateCommandQueue();
    CreateSwapChain(hWnd, width, height);
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

void WindowManager::CreateCommandQueue()
{
    D3D12_COMMAND_QUEUE_DESC queueDesc = {};
    queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    GFX_THROW_INFO_ONLY(Engine::Device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_commandQueue)));
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
    GFX_THROW_INFO_ONLY(Engine::Factory->CreateSwapChainForHwnd(m_commandQueue, hWnd, &swapChainDesc, nullptr, nullptr, &swapChain));

    GFX_THROW_INFO_ONLY(Engine::Factory->MakeWindowAssociation(hWnd, DXGI_MWA_NO_ALT_ENTER));

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
    CreateCommandList();
    CreateSyncObj();
}

#pragma region LoadAssetsFunction

void WindowManager::CreateCommandList()
{
    GFX_THROW_INFO_ONLY(Engine::Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_commandAllocator, nullptr, IID_PPV_ARGS(&m_commandList)));
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
    GFX_THROW_INFO_ONLY(m_commandList->Reset(m_commandAllocator, nullptr));

    // Paramètre l'affichage pour fonctionner avec une liste de triangle
    m_commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    /* AJOUT DES COMMANDES */

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
    MeshRenderer* mr = r2->GetComponent<MeshRenderer>();
    
    // POUR LES TEXTURES
    /*ShaderTexture* shader = (ShaderTexture*)(r2->GetComponent<MeshRenderer>()->m_shader);
    ConstantBufferSR* constBuffer = (ConstantBufferSR*)shader->m_constBuffer;
    constBuffer->CreateTexture(m_commandList);
    ID3D12DescriptorHeap* descriptorHeaps[] = { constBuffer->m_cbvHeapDesc, shader->m_samplerHeap };*/

    // POUR LES COULEURS
    ShaderColor* shader[] = {
        (ShaderColor*)(r1->GetComponent<MeshRenderer>()->m_shader),
        (ShaderColor*)(r2->GetComponent<MeshRenderer>()->m_shader)
    };
    ConstantBuffer* constBuffer[] = {
        shader[0]->m_constBuffer,
        shader[1]->m_constBuffer
    };
    ID3D12DescriptorHeap* descriptorHeaps[][1] = {
        {constBuffer[0]->m_cbvHeapDesc},
        {constBuffer[1]->m_cbvHeapDesc}
    };

    for (int i = 0; i < _countof(shader); ++i)
    {
        m_commandList->SetGraphicsRootSignature(mr->m_shader->m_rootSignature);// Ajout de la Root Signature
        m_commandList->SetPipelineState(mr->m_shader->m_pipelineState);// Ajout de la pipeline de rendu
        
        m_commandList->SetDescriptorHeaps(_countof(descriptorHeaps[i]), descriptorHeaps[i]);

        D3D12_GPU_DESCRIPTOR_HANDLE srv = constBuffer[i]->m_cbvHeapDesc->GetGPUDescriptorHandleForHeapStart();
        //D3D12_GPU_DESCRIPTOR_HANDLE sampler = shader->m_samplerHeap->GetGPUDescriptorHandleForHeapStart();

        m_commandList->SetGraphicsRootDescriptorTable(0, srv);
        //m_commandList->SetGraphicsRootConstantBufferView(1, constBuffer->m_buffer->GetGPUVirtualAddress());
        //m_commandList->SetGraphicsRootDescriptorTable(2, sampler);
        
        m_commandList->IASetVertexBuffers(0, 1, &mr->m_mesh->m_vertexBuffer->m_vertexBufferView);// Ajout des vertex buffer (ici 1 seul)
        m_commandList->IASetIndexBuffer(&mr->m_mesh->m_indexBuffer->m_indexBufferView);// Ajout des index buffer (ici 1 seul)
        m_commandList->DrawIndexedInstanced(mr->m_mesh->m_indexBuffer->m_nbVertex, 1, 0, 0, 0);// Affichage (avec toujours une seule instance)
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