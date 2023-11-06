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









void Engine::Draw(Entity e)
{
}