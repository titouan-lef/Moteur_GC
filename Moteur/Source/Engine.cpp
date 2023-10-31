#include "Engine.h"

Engine::Engine()
{
	this->m_Instance = this;
}

Engine::~Engine()
{
}

Engine* Engine::m_Instance = nullptr;
ID3D12Device* Engine::Device = nullptr;