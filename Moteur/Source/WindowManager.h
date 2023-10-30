#pragma once
#include "framwork.h"
#include "DxgiInfoManager.h"

class WindowManager
{
public:
    WindowManager(UINT width, UINT height);
    virtual ~WindowManager();

    void OnInit(UINT width, UINT height, HWND hWnd);
    void OnUpdate();
    void OnRender();
    void OnDestroy();

    // Samples override the event handlers to handle specific messages.
    virtual void OnKeyDown(UINT8 /*key*/) {}
    virtual void OnKeyUp(UINT8 /*key*/) {}

    static std::string ConvertWCharTToString(const wchar_t* wcharArray)
    {
        // Obtenir la taille n�cessaire pour la cha�ne de destination
        int size_needed = WideCharToMultiByte(CP_UTF8, 0, wcharArray, -1, NULL, 0, NULL, NULL);
        std::string result_string(size_needed, 0);

        // Convertir la cha�ne wchar_t* en std::string
        WideCharToMultiByte(CP_UTF8, 0, wcharArray, -1, &result_string[0], size_needed, NULL, NULL);

        return result_string;
    };

private:
#ifndef  NDEBUG
    DxgiInfoManager infoManager;
#endif // ! NDEBUG

    static const UINT FrameCount = 2;

    // Viewport dimensions.
    float m_aspectRatio;

    // Adapter info.
    bool m_useWarpDevice;

    // Pipeline objects.
    CD3DX12_VIEWPORT m_viewport = {};
    CD3DX12_RECT m_scissorRect = {};
    IDXGISwapChain3* m_swapChain = nullptr;
    ID3D12Device* m_device = nullptr;
    ID3D12Resource* m_renderTargets[FrameCount] = {};
    ID3D12CommandAllocator* m_commandAllocator = nullptr;
    ID3D12CommandQueue* m_commandQueue = nullptr;
    ID3D12RootSignature* m_rootSignature = nullptr;
    ID3D12DescriptorHeap* m_rtvHeap = nullptr;
   
    ID3D12DescriptorHeap* m_cbvHeap = nullptr;


    ID3D12PipelineState* m_pipelineState = nullptr;
    ID3D12GraphicsCommandList* m_commandList = nullptr;
    UINT m_rtvDescriptorSize;
    UINT m_cbvDescriptorSize;
    // App resources.

    ID3D12Resource* m_vertexBuffer = nullptr;
    D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView = {};

    ID3D12Resource* m_constBuffer = nullptr;
    ID3D12DescriptorHeap* cbvSrvUavHeap = nullptr;
    D3D12_CONSTANT_BUFFER_VIEW_DESC m_constBufferView = {};

    // Synchronization objects.
    UINT m_frameIndex = -1;
    HANDLE m_fenceEvent = {};
    ID3D12Fence* m_fence = {};
    UINT64 m_fenceValue = -1;

    // Gestion des vertices
    std::vector<Vertex> m_vertices = { {  } };

    void GetHardwareAdapter(
        _In_ IDXGIFactory1* pFactory,
        _Outptr_result_maybenull_ IDXGIAdapter1** ppAdapter,
        bool requestHighPerformanceAdapter = false);

    void LoadPipeline(UINT width, UINT height, HWND hWnd);

    void SetupDebugLayer();
    void CreateD3DDevice(IDXGIFactory4* factory);
    IDXGIFactory4* CreateDXGIFactory();
    void CreateCommandQueue();
    void CreateSwapChain(HWND hWnd, UINT width, UINT height, IDXGIFactory4* factory);
    void CreateDescriptorHeaps();
    void CreateFrameResources();
    void CreateCommandAllocator();




    void LoadAssets();

    void CreateRootSignature();
    void CreateVertexBuffer();
    void CreateConstantBuffer();
    void CreateSyncObj();



    void PopulateCommandList();
    void WaitForPreviousFrame();


    void GetAssetsPath(_Out_writes_(pathSize) WCHAR* path, UINT pathSize)
    {
        if (path == nullptr)
        {
            throw std::exception();
        }

        DWORD size = GetModuleFileName(nullptr, path, pathSize);
        if (size == 0 || size == pathSize)
        {
            // Method failed or path was truncated.
            throw std::exception();
        }

        WCHAR* lastSlash = wcsrchr(path, L'\\');
        if (lastSlash)
        {
            *(lastSlash + 1) = L'\0';
        }
    }
};
