#pragma once
#include "WindowManagerHelper.h"
#include "EngineException.h"
#include "GameObject.h"
#include "DxgiInfoManager.h"
class WindowManager
{
public:
#pragma region EXCEPTION
   class Exception : public EngineException {
        using EngineException::EngineException;
    };
    class HrException : public Exception {
    public:
        HrException(int line, const char* file, HRESULT hr, std::vector<std::string> infoMsgs = {}) noexcept;
        const char* what() const noexcept override;
        const char* GetType() const noexcept override;
        HRESULT GetErrorCode() const noexcept;
        std::string GetErrorString() const noexcept;
        std::string GetErrorDescription() const noexcept;
        std::string GetErrorInfo() const noexcept;
    private:
        HRESULT hr;
        std::string info;
    };
    class InfoException : public Exception
    {
    public:
        InfoException(int line, const char* file, std::vector<std::string> infoMsgs) noexcept;
        const char* what() const noexcept override;
        const char* GetType() const noexcept override;
        std::string GetErrorInfo() const noexcept;
    private:
        std::string info;
    };
    class DeviceRemovedException : public HrException {
        using HrException::HrException;
    public:
        const char* GetType()const noexcept override;
    private:
        std::string reason;
    };
#pragma endregion EXCEPTION
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
    static wchar_t* convertCharArrayToLPCWSTR(const char* charArray)
    {
        wchar_t* wString = new wchar_t[4096];
        MultiByteToWideChar(CP_ACP, 0, charArray, -1, wString, 4096);
        return wString;
    }

private:
#ifndef  NDEBUG
    DxgiInfoManager infoManager;
#endif // ! NDEBUG
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
   
    ID3D12DescriptorHeap* m_cbvHeap = nullptr;


    ID3D12PipelineState* m_pipelineState = nullptr;
    ID3D12GraphicsCommandList* m_commandList = nullptr;
    UINT m_rtvDescriptorSize;
    UINT m_cbvDescriptorSize;
    // App resources.

    ID3D12Resource* m_vertexBuffer = nullptr;
    D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView = {};

    // Synchronization objects.
    UINT m_frameIndex = -1;
    HANDLE m_fenceEvent = {};
    ID3D12Fence* m_fence = {};
    UINT64 m_fenceValue = -1;

    // Gestion des vertices
    static std::vector<GameObject*> m_gameObjects;
    std::vector<Vertex> m_vertices = { {  } };

    std::wstring GetAssetFullPath(LPCWSTR assetName);

    void GetHardwareAdapter(
        _In_ IDXGIFactory1* pFactory,
        _Outptr_result_maybenull_ IDXGIAdapter1** ppAdapter,
        bool requestHighPerformanceAdapter = false);

    void LoadPipeline(UINT width, UINT height, HWND hWnd);
    void LoadAssets();
    void PopulateCommandList();
    void WaitForPreviousFrame();

    void SetupDebugLayer();


    void CreateD3DDevice(IDXGIFactory4* factory);
    IDXGIFactory4* CreateDXGIFactory();
    void CreateCommandQueue();

    void CreateSwapChain(HWND hWnd,UINT width, UINT height, IDXGIFactory4* factory);

    void CreateDescriptorHeaps();
    void CreateFrameResources();

    void CreateCommandAllocator();
    //juste un convertisseur


};

