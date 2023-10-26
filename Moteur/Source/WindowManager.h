#pragma once
#include "framwork.h"
#include "WindowManagerHelper.h"
#include "GameObject.h"

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

    static void AddGameObject(GameObject* go);

private:
    static const UINT FrameCount = 2;

    // Viewport dimensions.
    float m_aspectRatio;

    // Adapter info.
    bool m_useWarpDevice;

    // Root assets path.
    std::wstring m_assetsPath;

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
    ID3D12PipelineState* m_pipelineState = nullptr;
    ID3D12GraphicsCommandList* m_commandList = nullptr;
    UINT m_rtvDescriptorSize;

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
    static std::vector<GameObject*> m_gameObjects;
    std::vector<Vertex> m_vertices = { {  } };

    void GetHardwareAdapter(
        _In_ IDXGIFactory1* pFactory,
        _Outptr_result_maybenull_ IDXGIAdapter1** ppAdapter,
        bool requestHighPerformanceAdapter = false);

    void LoadPipeline(UINT width, UINT height, HWND hWnd);
    void LoadAssets();
    void PopulateCommandList();
    void WaitForPreviousFrame();

};

