#include "WindowManager.h"
#include <sstream>
#include <iostream>
#include <string>
#include "dxerr.h"
#include <fstream>

#define GFX_EXCEPT_NOINFO(hr) WindowManager::HrException( __LINE__,__FILE__,(hr) )
#define GFX_THROW_NOINFO(hrcall) if( FAILED( hr = (hrcall) ) ) throw WindowManager::HrException( __LINE__,__FILE__,hr )

#ifndef NDEBUG
#define GFX_EXCEPT(hr) WindowManager::HrException( __LINE__,__FILE__,(hr),infoManager.GetMessages() )
#define GFX_THROW_INFO(hrcall) infoManager.Set(); if( FAILED( hr = (hrcall) ) ) throw GFX_EXCEPT(hr)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) WindowManager::DeviceRemovedException( __LINE__,__FILE__,(hr),infoManager.GetMessages() )
#define GFX_THROW_INFO_ONLY(call) infoManager.Set(); (call); {auto v = infoManager.GetMessages(); if(!v.empty()) {throw WindowManager::InfoException( __LINE__,__FILE__,v);}}
#else
#define GFX_EXCEPT(hr) WindowManager::HrException( __LINE__,__FILE__,(hr) )
#define GFX_THROW_INFO(hrcall) GFX_THROW_NOINFO(hrcall)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) WindowManager::DeviceRemovedException( __LINE__,__FILE__,(hr) )
#define GFX_THROW_INFO_ONLY(call) (call)
#endif

std::vector<GameObject*> WindowManager::m_gameObjects;


WindowManager::WindowManager(UINT width, UINT height) :
    m_useWarpDevice(false),
    m_frameIndex(0),
    m_viewport(0.0f, 0.0f, static_cast<float>(width), static_cast<float>(height)),
    m_scissorRect(0, 0, static_cast<LONG>(width), static_cast<LONG>(height)),
    m_rtvDescriptorSize(0)
{
    WCHAR assetsPath[512];
    GetAssetsPath(assetsPath, _countof(assetsPath));
    m_assetsPath = assetsPath;

    m_aspectRatio = static_cast<float>(width) / static_cast<float>(height);
}

WindowManager::~WindowManager()
{
    m_vertices.clear();
    
}

void WindowManager::OnInit(UINT width, UINT height, HWND hWnd)
{
    texture = new Shaders();
    LoadPipeline(width, height, hWnd);
    LoadAssets();
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
#pragma region  SOUS_FONCTIONS_LOAD_PIPELINE
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
    if (m_useWarpDevice)
    {
        IDXGIAdapter* warpAdapter;
        GFX_THROW_INFO_ONLY(factory->EnumWarpAdapter(IID_PPV_ARGS(&warpAdapter)));
        GFX_THROW_INFO_ONLY(D3D12CreateDevice(warpAdapter, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_device)));
    }
    else
    {
        IDXGIAdapter1* hardwareAdapter;
        GetHardwareAdapter(factory, &hardwareAdapter);
        GFX_THROW_INFO_ONLY(D3D12CreateDevice(hardwareAdapter, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_device)));
    }
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

    texture->CreateHeap(m_device);
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

// Load the sample assets.
void WindowManager::LoadAssets()
{
    texture->CreateSignature(m_device);
    // Create the pipeline state, which includes compiling and loading shaders.

    //texture->PipelineStateTexture(m_device, &m_pipelineState, L"source/shadersTexture.hlsl");
    texture->PipelineStateTexture(m_device, &m_pipelineState, L"source/shadersTexture.hlsl");


    // Create the command list.
    GFX_THROW_INFO_ONLY(m_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_commandAllocator, m_pipelineState, IID_PPV_ARGS(&m_commandList)));


    // Create the vertex buffer.
    {
        auto tmp1 = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
        auto tmp2 = CD3DX12_RESOURCE_DESC::Buffer(sizeof(Vertex));

        GFX_THROW_INFO_ONLY(m_device->CreateCommittedResource(
            &tmp1,
            D3D12_HEAP_FLAG_NONE,
            &tmp2,
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(&m_vertexBuffer)));
    }
    

    // Create the texture.
    texture->CreateTexture(m_device, m_commandList, L"source/pierre.jfif");

    // Close the command list and execute it to begin the initial GPU setup.
    GFX_THROW_INFO_ONLY(m_commandList->Close());
    ID3D12CommandList* ppCommandLists[] = { m_commandList };
    m_commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);


    // Create synchronization objects and wait until assets have been uploaded to the GPU.
    {
        GFX_THROW_INFO_ONLY(m_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence)));
        m_fenceValue = 1;

        // Create an event handle to use for frame synchronization.
        m_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
        if (m_fenceEvent == nullptr)
        {
            GFX_THROW_INFO_ONLY(HRESULT_FROM_WIN32(GetLastError()));
        }

        // Wait for the command list to execute; we are reusing the same command 
        // list in our main loop but for now, we just want to wait for setup to 
        // complete before continuing.
        WaitForPreviousFrame();
    }
}

// Update frame-based values.
void WindowManager::OnUpdate()
{
    //for checking result
    HRESULT hr;

    m_vertices.clear();

    for (GameObject* go : m_gameObjects)
    {
        const std::vector<Vertex> vertices = go->GetVertices();

        for (Vertex vertex : vertices)
        {
            vertex.m_position.y *= m_aspectRatio;
            m_vertices.push_back(vertex);
        }
    }

    const UINT vertexBufferSize = m_vertices.size() * sizeof(Vertex);

    // Copy the triangle data to the vertex buffer.
    UINT8* pVertexDataBegin;
    CD3DX12_RANGE readRange(0, 0);        // We do not intend to read from this resource on the CPU.
    GFX_THROW_INFO(m_vertexBuffer->Map(0, &readRange, reinterpret_cast<void**>(&pVertexDataBegin)));
    memcpy(pVertexDataBegin, m_vertices.data(), vertexBufferSize);
    m_vertexBuffer->Unmap(0, nullptr);

    // Initialize the vertex buffer view.
    m_vertexBufferView.BufferLocation = m_vertexBuffer->GetGPUVirtualAddress();
    m_vertexBufferView.StrideInBytes = sizeof(Vertex);
    m_vertexBufferView.SizeInBytes = vertexBufferSize;
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

    std::ofstream logFile("log.txt");
    UINT width = texture->imageWidth;
    UINT height = texture->imageHeight;

    if (logFile.is_open()) {
        logFile << width << std::endl;
        logFile << height << std::endl;
        logFile.close(); 
    }

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
    m_commandList->SetGraphicsRootSignature(texture->m_rootSignature);

    //Texture
    texture->UpdateHeap(m_commandList);


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


    // WAITING FOR THE FRAME TO COMPLETE BEFORE CONTINUING IS NOT BEST PRACTICE.
    // This is code implemented as such for simplicity. The D3D12HelloFrameBuffering
    // sample illustrates how to use fences for efficient resource usage and to
    // maximize GPU utilization.

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


// Helper function for resolving the full path of assets.
std::wstring WindowManager::GetAssetFullPath(LPCWSTR assetName)
{
    return m_assetsPath + assetName;
}

// Helper function for acquiring the first available hardware adapter that supports Direct3D 12.
// If no such adapter can be found, *ppAdapter will be set to nullptr.
_Use_decl_annotations_
void WindowManager::GetHardwareAdapter(
    IDXGIFactory1* pFactory,
    IDXGIAdapter1** ppAdapter,
    bool requestHighPerformanceAdapter)
{
    *ppAdapter = nullptr;

    IDXGIAdapter1* adapter = nullptr;

    IDXGIFactory6* factory6 = nullptr;
    if (SUCCEEDED(pFactory->QueryInterface(IID_PPV_ARGS(&factory6))))
    {
        for (
            UINT adapterIndex = 0;
            SUCCEEDED(factory6->EnumAdapterByGpuPreference(
                adapterIndex,
                requestHighPerformanceAdapter == true ? DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE : DXGI_GPU_PREFERENCE_UNSPECIFIED,
                IID_PPV_ARGS(&adapter)));
            ++adapterIndex)
        {
            DXGI_ADAPTER_DESC1 desc;
            adapter->GetDesc1(&desc);

            if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
            {
                // Don't select the Basic Render Driver adapter.
                // If you want a software adapter, pass in "/warp" on the command line.
                continue;
            }

            // Check to see whether the adapter supports Direct3D 12, but don't create the
            // actual device yet.
            if (SUCCEEDED(D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr)))
            {
                break;
            }
        }
    }

    if (adapter == nullptr)
    {
        for (UINT adapterIndex = 0; SUCCEEDED(pFactory->EnumAdapters1(adapterIndex, &adapter)); ++adapterIndex)
        {
            DXGI_ADAPTER_DESC1 desc;
            adapter->GetDesc1(&desc);

            if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
            {
                // Don't select the Basic Render Driver adapter.
                // If you want a software adapter, pass in "/warp" on the command line.
                continue;
            }

            // Check to see whether the adapter supports Direct3D 12, but don't create the
            // actual device yet.
            if (SUCCEEDED(D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr)))
            {
                break;
            }
        }
    }

    *ppAdapter = adapter;
}


void WindowManager::AddGameObject(GameObject* go)
{
    m_gameObjects.push_back(go);
}


WindowManager::HrException::HrException(int line, const char* file, HRESULT hr, std::vector<std::string> infoMsgs) noexcept : Exception(line, file), hr(hr)
{
    m_line = line;
    // join all info messages with newlines into single string
    for (const auto& m : infoMsgs)
    {
        info += m;
        info.push_back('\n');
    }
    // remove final newline if exists
    if (!info.empty())
    {
        info.pop_back();
    }
}

const char* WindowManager::HrException::what() const noexcept
{
   
    std::ostringstream oss;
    oss << GetType() << std::endl

        << "[Error Code] 0x" << std::hex << std::uppercase << GetErrorCode()
        << std::dec << " (" << (unsigned long)GetErrorCode() << ")" << std::endl
        << "[Error String] " << GetErrorString() << std::endl
        << "[Description] " << GetErrorDescription() << std::endl;


    if (!info.empty())
    {
        oss << "\n[Error Info]\n" << GetErrorInfo() << std::endl << std::endl;
    }
    oss << GetOriginString();
    whatBuffer = oss.str();
    return whatBuffer.c_str();
}

const char* WindowManager::HrException::GetType() const noexcept
{
    return "Graphics Exception";
}


HRESULT WindowManager::HrException::GetErrorCode() const noexcept
{
    return hr;
}

std::string WindowManager::HrException::GetErrorString() const noexcept
{
    std::wstring wStr = DXGetErrorString(hr);

    int length = WideCharToMultiByte(CP_UTF8, 0, wStr.c_str(), -1, nullptr, 0, nullptr, nullptr);
    std::vector<char> buffer(length);
    WideCharToMultiByte(CP_UTF8, 0, wStr.c_str(), -1, buffer.data(), length, nullptr, nullptr);

    return std::string(buffer.data());
}


std::string WindowManager::HrException::GetErrorDescription() const noexcept
{
    wchar_t wbuf[512];
    DXGetErrorDescription(hr, wbuf, sizeof(wbuf) / sizeof(wchar_t));
    return ConvertWStringToString(wbuf);
}

std::string WindowManager::HrException::GetErrorInfo() const noexcept
{
    return info;
}

const char* WindowManager::DeviceRemovedException::GetType() const noexcept
{
    return "Graphics Exception [Device Removed] (DXGI_ERROR_DEVICE_REMOVED)";
}

WindowManager::InfoException::InfoException(int line, const char* file, std::vector<std::string> infoMsgs) noexcept
    :
    Exception(line, file)
{
    
    // join all info messages with newlines into single string
    for (const auto& m : infoMsgs)
    {
        info += m;
        info.push_back('\n');
    }
    // remove final newline if exists
    if (!info.empty())
    {
        info.pop_back();
    }
}

const char* WindowManager::InfoException::what() const noexcept
{
    std::ostringstream oss;
    oss << GetType() << std::endl
        << "\n[Error Info]\n" << GetErrorInfo() << std::endl << std::endl;
    oss << GetOriginString();
    whatBuffer = oss.str();
    return whatBuffer.c_str();
}

const char* WindowManager::InfoException::GetType() const noexcept
{
    return "Graphics Info Exception";
}

std::string WindowManager::InfoException::GetErrorInfo() const noexcept
{
    return info;
}