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

    static const UINT FrameCount = 2;// Nombre de "surface de dessin" (= render target) que la Swap Chain g�re pour l'application

    // Pipeline objects.
    ID3D12Device* m_device = nullptr;// P�riph�rique de rendu
    ID3D12CommandQueue* m_commandQueue = nullptr;
    IDXGISwapChain3* m_swapChain = nullptr;
    ID3D12DescriptorHeap* m_rtvHeap = nullptr;// Tas contenant les emplacements pr�vu pour les "surfaces de dessins" (= render target)
    UINT m_rtvDescriptorSize = 0;// Taille d'un emplacements pr�vu pour les "surfaces de dessins"
    ID3D12Resource* m_renderTargets[FrameCount] = {};// Tableau contenant les surfaces de dessins
    ID3D12CommandAllocator* m_commandAllocator = nullptr;// Allocations de stockage pour les commandes du GPU
    ID3D12RootSignature* m_rootSignature = nullptr;// M�canisme qui d�finit comment les shaders acc�dent aux ressources graphiques
    ID3D12PipelineState* m_pipelineState = nullptr;// Sp�cifie comment la pipeline de rendu doit fonctionner pour chaque rendu
    ID3D12GraphicsCommandList* m_commandList = nullptr;
    D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView = {};// Indique au GPU comment interpr�ter les donn�es du vertex buffer
    D3D12_CONSTANT_BUFFER_VIEW_DESC m_constBufferView = {};// Indique au GPU comment interpr�ter les donn�es du constant buffer
    ID3D12DescriptorHeap* m_cbvSrvUavHeap = nullptr;

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

    IDXGIFactory4* CreateDXGIFactory();// Cr�ation de l'objet qui permet les interactions DirectX/GPU
    void CreateD3DDevice(IDXGIFactory4* factory);// Cr�ation du p�riph�rique de rendu
    void CreateCommandQueue();// Cr�ation de la file d'attente de commandes
    void CreateSwapChain(HWND hWnd, UINT width, UINT height, IDXGIFactory4* factory);// Cr�ation de la Swap chain
    void CreateDescriptorHeaps();// Cr�ation du tas de descripteurs RTV (Render Target View)
    void CreateFrameResources();// Cr�ation de la "surface de dessin" au bon endroit (= Render Target View)
    void CreateCommandAllocator();// Cr�ation des allocations de stockage pour les commandes du GPU


    void LoadAssets();

    void CreateRootSignature();// Cr�ation de la root signature
    void CreatePipelineState();// Cr�ation de la PSO (Pipeline State Object)
    ID3D12Resource* CreateBuffer(UINT bufferSize, const void* src);// Cr�ation d'un buffer
    void CreateVertexBuffer();// Cr�ation du vertex buffer
    void CreateConstantBuffer();// Cr�ation du constant buffer
    void CreateSyncObj();



    void PopulateCommandList();
    void WaitForPreviousFrame();

    // Recherche d'un adaptateur (ou une carte graphique) compatible avec DirectX 12
    bool IsValidAdapter(IDXGIAdapter1* adapter);
    bool AdapterFind(IDXGIFactory6* factory6, UINT adapterIndex, bool requestHighPerformanceAdapter, IDXGIAdapter1** pAdapter);
    IDXGIAdapter1* GetHardwareAdapter(IDXGIFactory1* pFactory, bool requestHighPerformanceAdapter = false);
};

