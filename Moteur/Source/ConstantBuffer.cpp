#include "ConstantBuffer.h"

ConstantBuffer::ConstantBuffer(ConstantBufferData* constBufferData, UINT nbDescriptor) : Buffer((sizeof(ConstantBufferData) + 255) & ~255, constBufferData)
{
	// Propriétés du tas de descripteurs CBV_SRV_UAV (Constant Buffer View - Shader Resource Views - Unordered Access Views) permettant d'accéder à des ressources du shader
	D3D12_DESCRIPTOR_HEAP_DESC cbvHeapDesc = {};
	cbvHeapDesc.NumDescriptors = nbDescriptor;
	cbvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	cbvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

	// Création du tas de descripteurs CBV_SRV_UAV dont le shader a besoin pour accéder aux différentes ressources
	Engine::m_Instance->Device->CreateDescriptorHeap(&cbvHeapDesc, IID_PPV_ARGS(&m_cbvHeapDesc));


	// Défini l'emplacement et la taille des données du constant buffer
	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
	cbvDesc.BufferLocation = m_buffer->GetGPUVirtualAddress();		// Localisation du constant buffer
	cbvDesc.SizeInBytes = m_bufferSize;								// Taille du constant buffer

	// Stockage du constant buffer view dans le tas
	Engine::m_Instance->Device->CreateConstantBufferView(&cbvDesc, m_cbvHeapDesc->GetCPUDescriptorHandleForHeapStart());
}

ConstantBuffer::~ConstantBuffer()
{
	delete m_cbvHeapDesc;
}

void ConstantBuffer::SetGraphicsRoot()
{
	D3D12_GPU_DESCRIPTOR_HANDLE srv = m_cbvHeapDesc->GetGPUDescriptorHandleForHeapStart();
	Engine::m_Instance->CmdList->SetGraphicsRootDescriptorTable(0, srv);
}




