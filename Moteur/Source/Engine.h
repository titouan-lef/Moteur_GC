#pragma once
#include "framwork.h"
#include "DxgiInfoManager.h"
#include "Entity.h"

class Engine 
{
public:
	Engine();
	virtual ~Engine();
	
	static Engine* m_Instance;

	//Get the instance of the engine
	static Engine* GetInstance() { return m_Instance; }


	static void Draw(Entity e);

	static Entity* GetPlayer() { return m_Instance->m_player; }

	static IDXGIFactory4* Factory;// Permet les interactions DirectX/GPU
	static ID3D12Device* Device;// Périphérique de rendu DirectX

	// Gestion des commandes
	static ID3D12GraphicsCommandList* CmdList;// Liste des commandes (dessin de géométrie, chargement de ressources, Configuration du pipeline graphique, ect) pour produire les rendus 3D
	static ID3D12CommandAllocator* CmdAllocator;// Allocations de stockage pour les commandes du GPU

private:
	// Gestion des erreurs
	#ifndef  NDEBUG
		static DxgiInfoManager infoManager;
	#endif

	Entity* m_player;

	static IDXGIFactory4* CreateDXGIFactory();// Création de l'objet qui permet les interactions DirectX/GPU
	static ID3D12Device* CreateD3DDevice();// Création du périphérique de rendu

	static ID3D12GraphicsCommandList* CreateCommandList();// Création de la liste de commandes
	static ID3D12CommandAllocator* CreateCommandAllocator();// Création des allocations de stockage pour les commandes du GPU


	// Recherche d'un adaptateur (ou une carte graphique) compatible avec DirectX 12
	static bool IsValidAdapter(IDXGIAdapter1* adapter);
	static bool AdapterFind(IDXGIFactory6* factory6, UINT adapterIndex, bool requestHighPerformanceAdapter, IDXGIAdapter1** pAdapter);
	static IDXGIAdapter1* GetHardwareAdapter(IDXGIFactory1* pFactory, bool requestHighPerformanceAdapter = false);
};