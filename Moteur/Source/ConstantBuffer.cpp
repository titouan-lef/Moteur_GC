#include "ConstantBuffer.h"
#include "Engine.h"

ConstantBuffer::ConstantBuffer(ConstantBufferData* constBufferData) : Buffer((sizeof(ConstantBufferData) + 255) & ~255, constBufferData)
{
	// Défini l'emplacement et la taille des données du constant buffer
	D3D12_CONSTANT_BUFFER_VIEW_DESC constBufferView = {};
	constBufferView.BufferLocation = m_buffer->GetGPUVirtualAddress();		// Localisation du constant buffer
	constBufferView.SizeInBytes = m_bufferSize;								// Taille du constant buffer

	// Propriétés du tas de descripteurs CBV_SRV_UAV (Constant Buffer View - Shader Resource Views - Unordered Access Views) permettant d'accéder à des ressources du shader
	D3D12_DESCRIPTOR_HEAP_DESC cbvSrvUavHeapDesc = {};
	cbvSrvUavHeapDesc.NumDescriptors = 1;
	cbvSrvUavHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	cbvSrvUavHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

	// Création du tas de descripteurs CBV_SRV_UAV dont le shader a besoin pour accéder aux différentes ressources
	ID3D12DescriptorHeap* cbvSrvUavHeap = nullptr;
	Engine::Device->CreateDescriptorHeap(&cbvSrvUavHeapDesc, IID_PPV_ARGS(&cbvSrvUavHeap));

	// Stockage du constant buffer view dans le tas
	Engine::Device->CreateConstantBufferView(&constBufferView, cbvSrvUavHeap->GetCPUDescriptorHandleForHeapStart());

	m_descriptorHeaps.push_back(cbvSrvUavHeap);
}