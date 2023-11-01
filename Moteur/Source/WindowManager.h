#pragma once
#include "DxgiInfoManager.h"
#include "Rectangle.h"// TO DO : A SUPPRIMER

#include "ConstantBuffer.h"
#include "Timer.h"
class WindowManager
{
public:
    WindowManager(UINT width, UINT height);
    virtual ~WindowManager();

    void OnInit(UINT width, UINT height, HWND hWnd);
    void CreateEntity();
    void OnUpdate();
    void OnRender();

    std::shared_ptr<ConstantBufferData> cbd2;
    // Gestion des touches
    virtual void OnKeyDown(UINT8 key) {}// Fonction réaliser lors de l'appui d'une touche
    virtual void OnKeyUp(UINT8 key) {}// Fonction réaliser lors du relachement d'une touche

private:
    // Gestion des erreurs
    #ifndef  NDEBUG
    DxgiInfoManager infoManager = {};
    #endif

    std::vector<std::shared_ptr<Entity>> m_entities;
    std::unique_ptr<Timer> m_entityTimer;

    // Gestion des fenêtres
    std::vector<CD3DX12_VIEWPORT> m_viewport = {};// Tableau contenant les dimensions de chaque fenêtre
    std::vector<CD3DX12_RECT> m_scissorRect = {};// Tableau contenant les rectangles qui définissent la zone où le rendu sera effectué pour chaque fenêtre

    // Gestion des commandes
    ID3D12GraphicsCommandList* m_commandList = nullptr;// Liste des commandes (dessin de géométrie, chargement de ressources, Configuration du pipeline graphique, ect) pour produire les rendus 3D
    ID3D12CommandQueue* m_commandQueue = nullptr;// File d'attente de commandes
    ID3D12CommandAllocator* m_commandAllocator = nullptr;// Allocations de stockage pour les commandes du GPU

    // Gestion des "surfaces de dessin" (= Render Target)
    static const UINT FrameCount = 2;// Nombre de "surfaces de dessin" que la Swap Chain gère pour l'application
    ID3D12Resource* m_renderTargets[FrameCount] = {};// Tableau contenant les "surfaces de dessin"
    ID3D12DescriptorHeap* m_rtvHeap = nullptr;// Tas contenant les emplacements prévu pour les "surfaces de dessin"
    UINT m_rtvDescriptorSize = 0;// Taille d'un emplacements prévu pour les "surfaces de dessin"
    IDXGISwapChain3* m_swapChain = nullptr;// Permet l'échange des "surfaces de dessin" dans les buffers

    // Gestion des shaders
    ID3D12PipelineState* m_pipelineState = nullptr;// Spécifie comment la pipeline de rendu doit fonctionner pour chaque rendu
    ID3D12RootSignature* m_rootSignature = nullptr;// Mécanisme qui définit comment les shaders accèdent aux ressources graphiques

    // Synchronisation du rendu
    UINT m_backBufferIndex = 0;// Indique quel est le back buffer actuel (l'indice varie ici de 0 à 1 car on utilise 2 buffers : le back et front buffer)
    UINT64 m_fenceId = 0;// Id de la frame actuelle
    ID3D12Fence* m_fence = {};// Mécanisme de synchronisation utilisé pour attendre la fin d'une série de commandes graphiques avant d'en exécuter d'autres

    MyRectangle* r1 = nullptr;//TO DO : A supprimer
    MyRectangle* r2 = nullptr;//TO DO : A supprimer


    void LoadPipeline(UINT width, UINT height, HWND hWnd);// Configuration de l'infrastructure de rendu

    void SetupDebugLayer();// Activation du debuggage de Direct3D
    IDXGIFactory4* CreateDXGIFactory();// Création de l'objet qui permet les interactions DirectX/GPU
    void CreateD3DDevice(IDXGIFactory4* factory);// Création du périphérique de rendu
    void CreateCommandQueue();// Création de la file d'attente de commandes
    void CreateSwapChain(HWND hWnd, UINT width, UINT height, IDXGIFactory4* factory);// Création de la Swap chain
    void CreateDescriptorHeaps();// Création du tas de descripteurs RTV (Render Target View)
    void CreateFrameResources();// Création de la "surface de dessin" au bon endroit (= Render Target View)
    void CreateCommandAllocator();// Création des allocations de stockage pour les commandes du GPU


    void LoadAssets();// Chargement des ressources nécessaire pour le rendu

    void CreateRootSignature();// Création de la root signature
    void CreatePipelineStateObject();// Création de la PSO (Pipeline State Object)
    void CreateCommandList();// Création de la liste de commandes
    void CreateSyncObj();// Création d'une infrastructure de synchronisation pour assurer que le GPU ait terminé son travail avant de passer à la frame suivante


    void PopulateCommandList();// Enregistre les commandes pour le rendu actuel
    void WaitForPreviousFrame();// Attend que la frame soit traitée avant de pouvoir être affiché

    // Recherche d'un adaptateur (ou une carte graphique) compatible avec DirectX 12
    bool IsValidAdapter(IDXGIAdapter1* adapter);
    bool AdapterFind(IDXGIFactory6* factory6, UINT adapterIndex, bool requestHighPerformanceAdapter, IDXGIAdapter1** pAdapter);
    IDXGIAdapter1* GetHardwareAdapter(IDXGIFactory1* pFactory, bool requestHighPerformanceAdapter = false);
};

