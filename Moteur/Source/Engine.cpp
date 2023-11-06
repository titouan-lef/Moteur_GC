#include "Engine.h"
#include "MyException.h"

DxgiInfoManager Engine::infoManager = {};

Engine* Engine::m_Instance = nullptr;

IDXGIFactory4* Engine::Factory = CreateDXGIFactory();
ID3D12Device* Engine::Device = CreateD3DDevice();
ID3D12CommandAllocator* Engine::CmdAllocator = CreateCommandAllocator();
ID3D12GraphicsCommandList* Engine::CmdList = CreateCommandList();

Engine::Engine()
{
	this->m_Instance = this;
}

Engine::~Engine()
{
}

IDXGIFactory4* Engine::CreateDXGIFactory()
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

ID3D12Device* Engine::CreateD3DDevice()
{
    ID3D12Device* device = nullptr;
    GFX_THROW_INFO_ONLY(D3D12CreateDevice(GetHardwareAdapter(Factory), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&device)));
    return device;
}

ID3D12GraphicsCommandList* Engine::CreateCommandList()
{
    ID3D12GraphicsCommandList* cmdList = nullptr;
    GFX_THROW_INFO_ONLY(Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, CmdAllocator, nullptr, IID_PPV_ARGS(&cmdList)));
    GFX_THROW_INFO_ONLY(cmdList->Close());// Indique que l'enregistrement des commandes est terminé et que le GPU peut les utiliser pour le rendu
    return cmdList;
}

ID3D12CommandAllocator* Engine::CreateCommandAllocator()
{
    ID3D12CommandAllocator* cmdAllocator = nullptr;
    GFX_THROW_INFO_ONLY(Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&cmdAllocator)));
    return cmdAllocator;
}



#pragma region HardwareAdapter
bool Engine::IsValidAdapter(IDXGIAdapter1* adapter)
{
    DXGI_ADAPTER_DESC1 desc;
    adapter->GetDesc1(&desc);
    return !(desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) && SUCCEEDED(D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr));
}

bool Engine::AdapterFind(IDXGIFactory6* factory6, UINT adapterIndex, bool requestHighPerformanceAdapter, IDXGIAdapter1** pAdapter)
{
    return SUCCEEDED(factory6->EnumAdapterByGpuPreference(
        adapterIndex,
        requestHighPerformanceAdapter == true ? DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE : DXGI_GPU_PREFERENCE_UNSPECIFIED,
        IID_PPV_ARGS(pAdapter)));
}

IDXGIAdapter1* Engine::GetHardwareAdapter(IDXGIFactory1* pFactory, bool requestHighPerformanceAdapter)
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









void Engine::BeforeRender()
{
    // Réinitialisaion pour enregistrer de nouvelles commandes pour la frame actuelle
    GFX_THROW_INFO_ONLY(Engine::CmdAllocator->Reset());
    GFX_THROW_INFO_ONLY(Engine::CmdList->Reset(Engine::CmdAllocator, nullptr));

    // Paramètre l'affichage pour fonctionner avec une liste de triangle
    Engine::CmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    /* AJOUT DES COMMANDES */

    // Ajout des différentes fenêtres et de leur zone de rendu
    Engine::CmdList->RSSetViewports(1, &m_viewport);          // Ajout des fenêtres (ici 1 seule)
    Engine::CmdList->RSSetScissorRects(1, &m_scissorRect); // Ajout des zones de rendu (ici 1 seule)

    // Indique que m_renderTargets[m_backBufferIndex] est prête à être utilisée comme "surfaces de dessin"
    CD3DX12_RESOURCE_BARRIER transition = CD3DX12_RESOURCE_BARRIER::Transition(m_renderTargets[m_backBufferIndex], D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);

    // Ajout des "surfaces de dessin" prêtes à être utilisées (ici 1 seule)
    Engine::CmdList->ResourceBarrier(1, &transition);

    // Ajout des "surfaces de dessin" au back buffer (ici 1 seule)
    CD3DX12_CPU_DESCRIPTOR_HANDLE renderTarget = CD3DX12_CPU_DESCRIPTOR_HANDLE(m_rtvHeap->GetCPUDescriptorHandleForHeapStart(), m_backBufferIndex, m_rtvDescriptorSize);
    Engine::CmdList->OMSetRenderTargets(1, &renderTarget, FALSE, nullptr);

    // Ajout de clearColor au premier plan pour effacer l'arrière plan par réécriture
    Engine::CmdList->ClearRenderTargetView(renderTarget, m_clearColor, 1, &m_scissorRect);// Ajout de clearColor aux "surfaces de dessin" (ici 1 seule)
}

void Engine::Render(Entity e)
{
    // POUR LES COULEURS
    MeshRenderer* meshRenderer[] = {
        r1->GetComponent<MeshRenderer>(),
        r2->GetComponent<MeshRenderer>()
    };
    Shader* shader[] = {
        meshRenderer[0]->m_shader,
       meshRenderer[1]->m_shader
    };
    ConstantBuffer* constBuffer[] = {
        shader[0]->m_constBuffer,
        shader[1]->m_constBuffer
    };

    for (int i = 0; i < _countof(shader); ++i)
    {
        Engine::CmdList->SetGraphicsRootSignature(shader[i]->m_rootSignature);// Ajout de la Root Signature
        Engine::CmdList->SetPipelineState(shader[i]->m_pipelineState);// Ajout de la pipeline de rendu
        
        Engine::CmdList->SetDescriptorHeaps(shader[i]->m_descriptorHeaps.size(), shader[i]->m_descriptorHeaps.data());

        constBuffer[i]->SetGraphicsRoot();

        Engine::CmdList->IASetVertexBuffers(0, 1, &meshRenderer[i]->m_mesh->m_vertexBuffer->m_vertexBufferView);// Ajout des vertex buffer (ici 1 seul)
        Engine::CmdList->IASetIndexBuffer(&meshRenderer[i]->m_mesh->m_indexBuffer->m_indexBufferView);// Ajout des index buffer (ici 1 seul)
        Engine::CmdList->DrawIndexedInstanced(meshRenderer[i]->m_mesh->m_indexBuffer->m_nbVertex, 1, 0, 0, 0);// Affichage (avec toujours une seule instance)
    }
}

void Engine::AfterRender()
{
    // Indique au back buffer les "surfaces de dessin" à ne plus utiliser
    transition = CD3DX12_RESOURCE_BARRIER::Transition(m_renderTargets[m_backBufferIndex], D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
    Engine::CmdList->ResourceBarrier(1, &transition);

    // Indique que l'enregistrement des commandes est terminé et que le GPU peut les utiliser pour le rendu
    GFX_THROW_INFO_ONLY(Engine::CmdList->Close());

    // Exécution des commandes (ici nous n'utilisons qu'une seul liste de comande)
    ID3D12CommandList* ppCommandLists[1] = { Engine::CmdList };
    m_commandQueue->ExecuteCommandLists(1, ppCommandLists);

    // Affichage de la frame.
    GFX_THROW_INFO_ONLY(m_swapChain->Present(1, 0));

    WaitForPreviousFrame();
}
