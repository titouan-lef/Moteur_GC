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


	static IDXGIFactory4* CreateDXGIFactory();// Création de l'objet qui permet les interactions DirectX/GPU
	static ID3D12Device* CreateD3DDevice();// Création du périphérique de rendu

	static ID3D12Device* Device;// Périphérique de rendu DirectX
	static IDXGIFactory4* Factory;// Permet les interactions DirectX/GPU

private:
	// Gestion des erreurs
	#ifndef  NDEBUG
		static DxgiInfoManager infoManager;
	#endif

	// Recherche d'un adaptateur (ou une carte graphique) compatible avec DirectX 12
	static bool IsValidAdapter(IDXGIAdapter1* adapter);
	static bool AdapterFind(IDXGIFactory6* factory6, UINT adapterIndex, bool requestHighPerformanceAdapter, IDXGIAdapter1** pAdapter);
	static IDXGIAdapter1* GetHardwareAdapter(IDXGIFactory1* pFactory, bool requestHighPerformanceAdapter = false);
};