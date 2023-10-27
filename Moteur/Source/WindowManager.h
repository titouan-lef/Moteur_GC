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

private:
#ifndef  NDEBUG
    DxgiInfoManager infoManager = {};
#endif // ! NDEBUG

    static const UINT FrameCount = 2;// Nombre de "surface de dessin" (= render target) que la Swap Chain gère pour l'application

    // Pipeline objects.
    ID3D12Device* m_device = nullptr;
    ID3D12CommandQueue* m_commandQueue = nullptr;
    IDXGISwapChain3* m_swapChain = nullptr;
    ID3D12DescriptorHeap* m_rtvHeap = nullptr;
    UINT m_rtvDescriptorSize = 0;
    ID3D12Resource* m_renderTargets[FrameCount] = {};
    ID3D12CommandAllocator* m_commandAllocator = nullptr;
    ID3D12RootSignature* m_rootSignature = nullptr;
    ID3D12PipelineState* m_pipelineState = nullptr;
    ID3D12GraphicsCommandList* m_commandList = nullptr;
    D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView = {};
    D3D12_CONSTANT_BUFFER_VIEW_DESC m_constBufferView = {};
    ID3D12DescriptorHeap* cbvSrvUavHeap = nullptr;

    CD3DX12_VIEWPORT m_viewport = {};
    CD3DX12_RECT m_scissorRect = {};

    //// Synchronization objects.
    UINT m_frameIndex = 0;
    ID3D12Fence* m_fence = {};
    UINT64 m_fenceValue = 1;
    HANDLE m_fenceEvent = {};

    // Gestion des vertices
    std::vector<Vertex> m_vertices = { {  } };

    void LoadPipeline(UINT width, UINT height, HWND hWnd);

    void SetupDebugLayer();// Activation du debuggage de Direct3D

    IDXGIFactory4* CreateDXGIFactory();// Création de l'objet qui permet les interactions DirectX/GPU
    void CreateD3DDevice(IDXGIFactory4* factory);// Création du périphérique de rendu
    void CreateCommandQueue();// Création de la file d'attente de commandes
    void CreateSwapChain(HWND hWnd, UINT width, UINT height, IDXGIFactory4* factory);// Création de la Swap chain
    void CreateDescriptorHeaps();// Création du tas de descripteurs RTV (Render Target View)
    void CreateFrameResources();
    void CreateCommandAllocator();




    void LoadAssets();

    void CreateRootSignature();
    void CreatePipelineState();
    void CreateVertexBuffer();
    void CreateConstantBuffer();
    void CreateSyncObj();



    void PopulateCommandList();
    void WaitForPreviousFrame();

    // Cherche un adaptateur (ou une carte graphique) compatible avec DirectX 12
    bool IsValidAdapter(IDXGIAdapter1* adapter);
    bool AdapterFind(IDXGIFactory6* factory6, UINT adapterIndex, bool requestHighPerformanceAdapter, IDXGIAdapter1** pAdapter);
    IDXGIAdapter1* GetHardwareAdapter(IDXGIFactory1* pFactory, bool requestHighPerformanceAdapter = false);
};

