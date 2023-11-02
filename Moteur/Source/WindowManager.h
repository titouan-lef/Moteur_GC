#pragma once
#include "DxgiInfoManager.h"
#include "Rectangle.h"// TO DO : A SUPPRIMER

#include "ConstantBuffer.h"
#include "Timer.h"
#include <vector>
class WindowManager
{
public:
    WindowManager(UINT width, UINT height);
    virtual ~WindowManager();

    void OnInit(UINT width, UINT height, HWND hWnd);
    void CreateEntity();
    void OnUpdate();
    void OnRender();

    std::vector<ConstantBuffer*> cb;
    // Gestion des touches
    virtual void OnKeyDown(UINT8 key) {}// Fonction r�aliser lors de l'appui d'une touche
    virtual void OnKeyUp(UINT8 key) {}// Fonction r�aliser lors du relachement d'une touche

private:
    // Gestion des erreurs
    #ifndef  NDEBUG
    DxgiInfoManager infoManager = {};
    #endif

    // Gestion des fen�tres (ici 1 seule)
    CD3DX12_VIEWPORT m_viewport = {};// Tableau contenant les dimensions de chaque fen�tre
    CD3DX12_RECT m_scissorRect = {};// Tableau contenant les rectangles qui d�finissent la zone o� le rendu sera effectu� pour chaque fen�tre
        std::vector<std::shared_ptr<Entity>> m_entities;
    std::unique_ptr<Timer> m_entityTimer;


    // Gestion des commandes
    ID3D12GraphicsCommandList* m_commandList = nullptr;// Liste des commandes (dessin de g�om�trie, chargement de ressources, Configuration du pipeline graphique, ect) pour produire les rendus 3D
    ID3D12CommandQueue* m_commandQueue = nullptr;// File d'attente de commandes
    ID3D12CommandAllocator* m_commandAllocator = nullptr;// Allocations de stockage pour les commandes du GPU

    // Gestion des "surfaces de dessin" (= Render Target)
    static const UINT FrameCount = 2;// Nombre de "surfaces de dessin" que la Swap Chain g�re pour l'application
    ID3D12Resource* m_renderTargets[FrameCount] = {};// Tableau contenant les "surfaces de dessin"
    ID3D12DescriptorHeap* m_rtvHeap = nullptr;// Tas contenant les emplacements pr�vu pour les "surfaces de dessin"
    UINT m_rtvDescriptorSize = 0;// Taille d'un emplacements pr�vu pour les "surfaces de dessin"
    IDXGISwapChain3* m_swapChain = nullptr;// Permet l'�change des "surfaces de dessin" dans les buffers

    // Gestion des shaders
    ID3D12PipelineState* m_pipelineState = nullptr;// Sp�cifie comment la pipeline de rendu doit fonctionner pour chaque rendu
    ID3D12RootSignature* m_rootSignature = nullptr;// M�canisme qui d�finit comment les shaders acc�dent aux ressources graphiques

    // Synchronisation du rendu
    UINT m_backBufferIndex = 0;// Indique quel est le back buffer actuel (l'indice varie ici de 0 � 1 car on utilise 2 buffers : le back et front buffer)
    UINT64 m_fenceId = 0;// Id de la frame actuelle
    ID3D12Fence* m_fence = {};// M�canisme de synchronisation utilis� pour attendre la fin d'une s�rie de commandes graphiques avant d'en ex�cuter d'autres

    const float m_clearColor[4] = { 0.0f, 0.2f, 0.4f, 1.0f };// Couleur du fond de la fen�tre

    MyRectangle* r1 = nullptr;//TO DO : A supprimer
    MyRectangle* r2 = nullptr;//TO DO : A supprimer


    void LoadPipeline(UINT width, UINT height, HWND hWnd);// Configuration de l'infrastructure de rendu

    void SetupDebugLayer();// Activation du debuggage de Direct3D
    IDXGIFactory4* CreateDXGIFactory();// Cr�ation de l'objet qui permet les interactions DirectX/GPU
    void CreateD3DDevice(IDXGIFactory4* factory);// Cr�ation du p�riph�rique de rendu
    void CreateCommandQueue();// Cr�ation de la file d'attente de commandes
    void CreateSwapChain(HWND hWnd, UINT width, UINT height, IDXGIFactory4* factory);// Cr�ation de la Swap chain
    void CreateDescriptorHeaps();// Cr�ation du tas de descripteurs RTV (Render Target View)
    void CreateFrameResources();// Cr�ation de la "surface de dessin" au bon endroit (= Render Target View)
    void CreateCommandAllocator();// Cr�ation des allocations de stockage pour les commandes du GPU


    void LoadAssets();// Chargement des ressources n�cessaire pour le rendu

    void CreateRootSignature();// Cr�ation de la root signature
    void CreatePipelineStateObject();// Cr�ation de la PSO (Pipeline State Object)
    void CreateCommandList();// Cr�ation de la liste de commandes
    void CreateSyncObj();// Cr�ation d'une infrastructure de synchronisation pour assurer que le GPU ait termin� son travail avant de passer � la frame suivante


    void PopulateCommandList();// Enregistre les commandes pour le rendu actuel
    void WaitForPreviousFrame();// Attend que la frame soit trait�e avant de pouvoir �tre affich�

    // Recherche d'un adaptateur (ou une carte graphique) compatible avec DirectX 12
    bool IsValidAdapter(IDXGIAdapter1* adapter);
    bool AdapterFind(IDXGIFactory6* factory6, UINT adapterIndex, bool requestHighPerformanceAdapter, IDXGIAdapter1** pAdapter);
    IDXGIAdapter1* GetHardwareAdapter(IDXGIFactory1* pFactory, bool requestHighPerformanceAdapter = false);
};

