#pragma once

#include "framwork.h"

class Engine 
{
public:
	Engine();
	virtual ~Engine();
	
	static Engine* m_Instance;

	//Get the instance of the engine
	static Engine* GetInstance() { return m_Instance; }

    // Périphérique de rendu DirectX
	static ID3D12Device* Device;
};