#pragma once
#include "framwork.h"
#include "DxgiInfoManager.h"
#include "Entity.h"

class Engine 
{
public:
	Engine();
	virtual ~Engine();

	static void Init();

	//Get the instance of the engine
	static Engine* GetInstance() { return m_Instance; }

	void Render(Entity* e);

	IDXGIFactory4* Factory = nullptr;// Permet les interactions DirectX/GPU
	ID3D12Device* Device = nullptr;// Périphérique de rendu DirectX

	// Gestion des commandes
	ID3D12GraphicsCommandList* CmdList = nullptr;// Liste des commandes (dessin de géométrie, chargement de ressources, Configuration du pipeline graphique, ect) pour produire les rendus 3D
	ID3D12CommandAllocator* CmdAllocator = nullptr;// Allocations de stockage pour les commandes du GPU

private:
	static Engine* m_Instance;

	// Gestion des erreurs
	#ifndef  NDEBUG
		DxgiInfoManager infoManager;
	#endif

	IDXGIFactory4* CreateDXGIFactory();// Création de l'objet qui permet les interactions DirectX/GPU
	ID3D12Device* CreateD3DDevice();// Création du périphérique de rendu

	ID3D12GraphicsCommandList* CreateCommandList();// Création de la liste de commandes
	ID3D12CommandAllocator* CreateCommandAllocator();// Création des allocations de stockage pour les commandes du GPU

	// Recherche d'un adaptateur (ou une carte graphique) compatible avec DirectX 12
	bool IsValidAdapter(IDXGIAdapter1* adapter);
	bool AdapterFind(IDXGIFactory6* factory6, UINT adapterIndex, bool requestHighPerformanceAdapter, IDXGIAdapter1** pAdapter);
	IDXGIAdapter1* GetHardwareAdapter(IDXGIFactory1* pFactory, bool requestHighPerformanceAdapter = false);
};