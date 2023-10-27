#include "WindowManager.h"
#include "Entity.h"
#include "MyException.h"

WindowManager::WindowManager(UINT width, UINT height) :
    m_viewport(0.0f, 0.0f, static_cast<float>(width), static_cast<float>(height)),
    m_scissorRect(0, 0, static_cast<LONG>(width), static_cast<LONG>(height))
{
}

WindowManager::~WindowManager()
{
    m_vertices.clear();
}

void WindowManager::OnInit(UINT width, UINT height, HWND hWnd)
{
    LoadPipeline(width, height, hWnd);
    LoadAssets();
    WaitForPreviousFrame();
}

// Load the rendering pipeline dependencies.
void WindowManager::LoadPipeline(UINT width, UINT height, HWND hWnd)
{
    // Enable the debug layer if in debug mode.
    SetupDebugLayer();

    // Create a DXGI factory.
    IDXGIFactory4* factory = CreateDXGIFactory();

    // Create the Direct3D device.
    CreateD3DDevice(factory);

    // Create the command queue.
    CreateCommandQueue();

    // Create and configure the swap chain.
    CreateSwapChain(hWnd, width, height, factory);

    // Create descriptor heaps.
    CreateDescriptorHeaps();

    // Create frame resources (render targets).
    CreateFrameResources();

    // Create a command allocator.
    CreateCommandAllocator();
}

#pragma region LoadPipelineFunction

// Enable the Direct3D debug layer (only in debug mode).
void WindowManager::SetupDebugLayer()
{
    #if defined(_DEBUG)
    ID3D12Debug* debugController;
    if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
    {
        debugController->EnableDebugLayer();
    }
    #endif
}

// Create a DXGI factory.
IDXGIFactory4* WindowManager::CreateDXGIFactory()
{
    UINT dxgiFactoryFlags = 0;

    #if defined(_DEBUG)
    // Enable the debug layer (requires the Graphics Tools "optional feature").
    // NOTE: Enabling the debug layer after device creation will invalidate the active device.
    {
        ID3D12Debug* debugController;
        if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
        {
            debugController->EnableDebugLayer();

            // Enable additional debug layers.
            dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
        }
    }
    #endif

    IDXGIFactory4* factory;
    GFX_THROW_INFO_ONLY(CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&factory)));

    return factory;
}

// Create the Direct3D device.
void WindowManager::CreateD3DDevice(IDXGIFactory4* factory)
{
    IDXGIAdapter1* hardwareAdapter = GetHardwareAdapter(factory);
    GFX_THROW_INFO_ONLY(D3D12CreateDevice(hardwareAdapter, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_device)));
}

// Create the command queue.
void WindowManager::CreateCommandQueue()
{
    D3D12_COMMAND_QUEUE_DESC queueDesc = {};
    queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    GFX_THROW_INFO_ONLY(m_device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_commandQueue)));
}

// Create and configure the swap chain.
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
    GFX_THROW_INFO_ONLY(factory->CreateSwapChainForHwnd(
        m_commandQueue, hWnd, &swapChainDesc, nullptr, nullptr, &swapChain
    ));

    GFX_THROW_INFO_ONLY(factory->MakeWindowAssociation(hWnd, DXGI_MWA_NO_ALT_ENTER));

    m_swapChain = (IDXGISwapChain3*)swapChain;
    m_frameIndex = m_swapChain->GetCurrentBackBufferIndex();
}

// Create descriptor heaps.
void WindowManager::CreateDescriptorHeaps()
{
    D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
    rtvHeapDesc.NumDescriptors = FrameCount;
    rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    GFX_THROW_INFO_ONLY(m_device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&m_rtvHeap)));
    m_rtvDescriptorSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
}

// Create frame resources (render targets).
void WindowManager::CreateFrameResources()
{
    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart());

    for (UINT n = 0; n < FrameCount; n++)
    {
        GFX_THROW_INFO_ONLY(m_swapChain->GetBuffer(n, IID_PPV_ARGS(&m_renderTargets[n])));
        m_device->CreateRenderTargetView(m_renderTargets[n], nullptr, rtvHandle);
        rtvHandle.Offset(1, m_rtvDescriptorSize);
    }
}

// Create a command allocator.
void WindowManager::CreateCommandAllocator()
{
    GFX_THROW_INFO_ONLY(m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_commandAllocator)));
}

#pragma endregion

// Load the sample assets.
void WindowManager::LoadAssets()
{
    // Create an root signature.
    CreateRootSignature();
    

    // Create the pipeline state, which includes compiling and loading shaders.
    CreatePipelineState();

    // Create the command list.
    GFX_THROW_INFO_ONLY(m_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_commandAllocator, m_pipelineState, IID_PPV_ARGS(&m_commandList)));

    // Command lists are created in the recording state, but there is nothing
    // to record yet. The main loop expects it to be closed, so close it now.
    GFX_THROW_INFO_ONLY(m_commandList->Close());

    // Create the vertex buffer.
    CreateVertexBuffer();

    // Cr�ation constant buffer
    CreateConstantBuffer();

    // Create synchronization objects 
    CreateSyncObj();
}

#pragma region LoadAssetsFunction
void WindowManager::CreateRootSignature()
{
    // Root parameter can be a table, root descriptor or root constants.
    CD3DX12_ROOT_PARAMETER slotRootParameter[1];

    // Create a single descriptor table of CBVs.
    CD3DX12_DESCRIPTOR_RANGE cbvTable;
    cbvTable.Init(
        D3D12_DESCRIPTOR_RANGE_TYPE_CBV,
        1, // Number of descriptors in table
        0);// base shader register arguments are bound to for this root parameter

    slotRootParameter[0].InitAsDescriptorTable(
        1, // Number of ranges
        &cbvTable); // Pointer to array of ranges

    // A root signature is an array of root parameters.
    CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc(1, slotRootParameter, 0,
        nullptr,
        D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

    // create a root signature with a single slot which points to a 
    // descriptor range consisting of a single constant buffer.
    ID3DBlob* serializedRootSig = nullptr;
    ID3DBlob* errorBlob = nullptr;
    HRESULT hr = D3D12SerializeRootSignature(
        &rootSigDesc,
        D3D_ROOT_SIGNATURE_VERSION_1,
        &serializedRootSig,
        &errorBlob
    );

    GFX_THROW_INFO_ONLY(m_device->CreateRootSignature(
        0,
        serializedRootSig->GetBufferPointer(),
        serializedRootSig->GetBufferSize(),
        IID_PPV_ARGS(&m_rootSignature))
    );
    //ID3DBlob* signature;
    //ID3DBlob* error;
}

void WindowManager::CreatePipelineState()
{
    ID3DBlob* vertexShader;
    ID3DBlob* pixelShader;

#if defined(_DEBUG)
    // Enable better shader debugging with the graphics debugging tools.
    UINT compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
    UINT compileFlags = 0;
#endif

    HRESULT hr;

    GFX_THROW_INFO(D3DCompileFromFile(L"Source/shaders.hlsl", nullptr, nullptr, "VSMain", "vs_5_0", compileFlags, 0, &vertexShader, nullptr));
    GFX_THROW_INFO(D3DCompileFromFile(L"Source/shaders.hlsl", nullptr, nullptr, "PSMain", "ps_5_0", compileFlags, 0, &pixelShader, nullptr));

    // Define the vertex input layout.
    D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
    };

    // Describe and create the graphics pipeline state object (PSO).
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
    GFX_THROW_INFO_ONLY(m_device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_pipelineState)));
}

void WindowManager::CreateVertexBuffer()
{
    m_vertices = {
        // Carr�
        { { -0.5f, 0.5f, 0 }, { 1.0f, 1.0f, 1.0f, 1.0f } },// Coin sup�rieur gauche
        { { 0.5f, 0.5f, 0 }, { 1.0f, 1.0f, 1.0f, 1.0f } },// Coin sup�rieur droit
        { { -0.5f, -0.5f, 0 }, { 1.0f, 1.0f, 1.0f, 1.0f } },// Coin inf�rieur gauche

        { { -0.5f, -0.5f, 0 }, { 1.0f, 1.0f, 1.0f, 1.0f } },// Coin inf�rieur gauche
        { { 0.5f, 0.5f, 0 }, { 1.0f, 1.0f, 1.0f, 1.0f } },// Coin sup�rieur droit
        { { 0.5f, -0.5f, 0 }, { 1.0f, 1.0f, 1.0f, 1.0f } },// Coin inf�rieur droit
    };

    const UINT vertexBufferSize = m_vertices.size() * sizeof(Vertex);


    auto tmp1 = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
    auto tmp2 = CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize);

    ID3D12Resource* m_vertexBuffer = nullptr;

    GFX_THROW_INFO_ONLY(m_device->CreateCommittedResource(
        &tmp1,
        D3D12_HEAP_FLAG_NONE,
        &tmp2,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&m_vertexBuffer)));

    // Copy the triangle data to the vertex buffer.
    UINT8* pVertexDataBegin;
    CD3DX12_RANGE readRange(0, 0);        // We do not intend to read from this resource on the CPU.
    GFX_THROW_INFO_ONLY(m_vertexBuffer->Map(0, &readRange, reinterpret_cast<void**>(&pVertexDataBegin)));
    memcpy(pVertexDataBegin, m_vertices.data(), vertexBufferSize);
    m_vertexBuffer->Unmap(0, nullptr);

    // Initialize the vertex buffer view.
    m_vertexBufferView.BufferLocation = m_vertexBuffer->GetGPUVirtualAddress();
    m_vertexBufferView.StrideInBytes = sizeof(Vertex);
    m_vertexBufferView.SizeInBytes = vertexBufferSize;
}

void WindowManager::CreateConstantBuffer()
{
    Entity e = Entity();

    struct ConstantBufferData
    {
        DirectX::XMMATRIX World;
    };

    ConstantBufferData constBufferData;
    e.m_Transform.UpdateMatrix();
    e.m_Transform.MoveByVector({ 0, 0, 0.5f });
    e.m_Transform.Rotate(0.5f, -0.25f, 0);
    e.m_Transform.UpdateMatrix();
    constBufferData.World = e.m_Transform.GetMatrixTranspose();

    const UINT constBufferSize = (sizeof(ConstantBufferData) + 255) & ~255;


    auto tmp1 = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
    auto tmp2 = CD3DX12_RESOURCE_DESC::Buffer(constBufferSize);

    ID3D12Resource* m_constBuffer = nullptr;

    GFX_THROW_INFO_ONLY(m_device->CreateCommittedResource(
        &tmp1,
        D3D12_HEAP_FLAG_NONE,
        &tmp2,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&m_constBuffer)));

    // Copy the triangle data to the vertex buffer.
    BYTE* mappedConstData = nullptr;
    GFX_THROW_INFO_ONLY(m_constBuffer->Map(0, nullptr, reinterpret_cast<void**>(&mappedConstData)));
    memcpy(mappedConstData, &constBufferData, constBufferSize);
    m_constBuffer->Unmap(0, nullptr);

    // Cr�ez un tas de descripteurs de type CBV_SRV_UAV
    D3D12_DESCRIPTOR_HEAP_DESC cbvSrvUavHeapDesc = {};
    cbvSrvUavHeapDesc.NumDescriptors = 1; // Un descripteur de Constant Buffer View (CBV)
    cbvSrvUavHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    cbvSrvUavHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

    m_device->CreateDescriptorHeap(&cbvSrvUavHeapDesc, IID_PPV_ARGS(&cbvSrvUavHeap));

    m_constBufferView.BufferLocation = m_constBuffer->GetGPUVirtualAddress();
    m_constBufferView.SizeInBytes = constBufferSize;

    m_device->CreateConstantBufferView(
        &m_constBufferView,
        cbvSrvUavHeap->GetCPUDescriptorHandleForHeapStart()
    );
}

void WindowManager::CreateSyncObj()
{
    GFX_THROW_INFO_ONLY(m_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence)));

    // Create an event handle to use for frame synchronization.
    m_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
    if (m_fenceEvent == nullptr)
    {
        GFX_THROW_INFO_ONLY(HRESULT_FROM_WIN32(GetLastError()));
    }
}
#pragma endregion

// Update frame-based values.
void WindowManager::OnUpdate()
{
    
}

// Render the scene.
void WindowManager::OnRender()
{
    // Record all the commands we need to render the scene into the command list.
    PopulateCommandList();

    // Execute the command list.
    ID3D12CommandList* ppCommandLists[] = { m_commandList };
    m_commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

    // Present the frame.
    GFX_THROW_INFO_ONLY(m_swapChain->Present(1, 0));

    WaitForPreviousFrame();
}

void WindowManager::OnDestroy()
{
    // Ensure that the GPU is no longer referencing resources that are about to be
    // cleaned up by the destructor.
    WaitForPreviousFrame();

    CloseHandle(m_fenceEvent);
}

void WindowManager::PopulateCommandList()
{
    // Command list allocators can only be reset when the associated 
    // command lists have finished execution on the GPU; apps should use 
    // fences to determine GPU execution progress.
    GFX_THROW_INFO_ONLY(m_commandAllocator->Reset());

    // However, when ExecuteCommandList() is called on a particular command 
    // list, that command list can then be reset at any time and must be before 
    // re-recording.
    GFX_THROW_INFO_ONLY(m_commandList->Reset(m_commandAllocator, m_pipelineState));

    // Set necessary state.
    m_commandList->SetGraphicsRootSignature(m_rootSignature);

    // Gestion Constant Buffer
    ID3D12DescriptorHeap* descriptorHeaps[] = { cbvSrvUavHeap };
    m_commandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);
    m_commandList->SetGraphicsRootDescriptorTable(0, cbvSrvUavHeap->GetGPUDescriptorHandleForHeapStart());



    m_commandList->RSSetViewports(1, &m_viewport);
    m_commandList->RSSetScissorRects(1, &m_scissorRect);

    // Indicate that the back buffer will be used as a render target.
    auto tmp1 = CD3DX12_RESOURCE_BARRIER::Transition(m_renderTargets[m_frameIndex], D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
    m_commandList->ResourceBarrier(1, &tmp1);

    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart(), m_frameIndex, m_rtvDescriptorSize);
    m_commandList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);

    // Record commands.
    const float clearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f };
    m_commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
    m_commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    m_commandList->IASetVertexBuffers(0, 1, &m_vertexBufferView);
    m_commandList->DrawInstanced(m_vertices.size(), 1, 0, 0);

    // Indicate that the back buffer will now be used to present.
    auto tmp2 = CD3DX12_RESOURCE_BARRIER::Transition(m_renderTargets[m_frameIndex], D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
    m_commandList->ResourceBarrier(1, &tmp2);

    GFX_THROW_INFO_ONLY(m_commandList->Close());
}

void WindowManager::WaitForPreviousFrame()
{
    // Signal and increment the fence value.
    const UINT64 fence = m_fenceValue;
    GFX_THROW_INFO_ONLY(m_commandQueue->Signal(m_fence, fence));
    m_fenceValue++;

    // Wait until the previous frame is finished.
    if (m_fence->GetCompletedValue() < fence)
    {
        GFX_THROW_INFO_ONLY(m_fence->SetEventOnCompletion(fence, m_fenceEvent));
        WaitForSingleObject(m_fenceEvent, INFINITE);
    }

    m_frameIndex = m_swapChain->GetCurrentBackBufferIndex();
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

// Helper function for acquiring the first available hardware adapter that supports Direct3D 12.
// If no such adapter can be found, *ppAdapter will be set to nullptr.
_Use_decl_annotations_
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