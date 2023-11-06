#pragma once
#include "DxgiInfoManager.h"
#include "Cube.h"
#include "Rectangle.h"// TO DO : A SUPPRIMER
#include "Timer.h"

class WindowManager
{
public:
    WindowManager(UINT width, UINT height);
    virtual ~WindowManager();

    void OnInit(UINT width, UINT height, HWND hWnd);
    void OnUpdate();
    void OnRender();

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

    std::vector<Entity*> m_entities;
    Timer* m_entityTimer;

    // Gestion des commandes
    ID3D12CommandQueue* m_commandQueue = nullptr;// File d'attente de commandes

    // Gestion des "surfaces de dessin" (= Render Target)
    static const UINT FrameCount = 2;// Nombre de "surfaces de dessin" que la Swap Chain g�re pour l'application
    ID3D12Resource* m_renderTargets[FrameCount] = {};// Tableau contenant les "surfaces de dessin"
    ID3D12DescriptorHeap* m_rtvHeap = nullptr;// Tas contenant les emplacements pr�vu pour les "surfaces de dessin"
    UINT m_rtvDescriptorSize = 0;// Taille d'un emplacements pr�vu pour les "surfaces de dessin"
    IDXGISwapChain3* m_swapChain = nullptr;// Permet l'�change des "surfaces de dessin" dans les buffers
    

    // Synchronisation du rendu
    UINT m_backBufferIndex = 0;// Indique quel est le back buffer actuel (l'indice varie ici de 0 � 1 car on utilise 2 buffers : le back et front buffer)
    UINT64 m_fenceId = 0;// Id de la frame actuelle
    ID3D12Fence* m_fence = {};// M�canisme de synchronisation utilis� pour attendre la fin d'une s�rie de commandes graphiques avant d'en ex�cuter d'autres

    const float m_clearColor[4] = { 0.0f, 0.2f, 0.4f, 1.0f };// Couleur du fond de la fen�tre


    Cube* r1 = nullptr;//TO DO : A supprimer
    Cube* r2 = nullptr;//TO DO : A supprimer

    void LoadPipeline(UINT width, UINT height, HWND hWnd);// Configuration de l'infrastructure de rendu

    void SetupDebugLayer();// Activation du debuggage de Direct3D
    void CreateCommandQueue();// Cr�ation de la file d'attente de commandes
    void CreateSwapChain(HWND hWnd, UINT width, UINT height);// Cr�ation de la Swap chain
    void CreateDescriptorHeaps();// Cr�ation du tas de descripteurs RTV (Render Target View)
    void CreateFrameResources();// Cr�ation de la "surface de dessin" au bon endroit (= Render Target View)

    void LoadAssets();// Chargement des ressources n�cessaire pour le rendu

    void CreateSyncObj();// Cr�ation d'une infrastructure de synchronisation pour assurer que le GPU ait termin� son travail avant de passer � la frame suivante


    void PopulateCommandList();// Enregistre les commandes pour le rendu actuel
    void WaitForPreviousFrame();// Attend que la frame soit trait�e avant de pouvoir �tre affich�
};
