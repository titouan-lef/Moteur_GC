#include "ConstantBuffer.h"

ConstantBuffer::ConstantBuffer(ConstantBufferData* constBufferData) : Buffer((sizeof(ConstantBufferData) + 255) & ~255, constBufferData)
{
	// D�fini l'emplacement et la taille des donn�es du constant buffer
	D3D12_CONSTANT_BUFFER_VIEW_DESC constBufferView = {};
	constBufferView.BufferLocation = m_buffer->GetGPUVirtualAddress();		// Localisation du constant buffer
	constBufferView.SizeInBytes = m_bufferSize;								// Taille du constant buffer

	// Propri�t�s du tas de descripteurs CBV_SRV_UAV (Constant Buffer View - Shader Resource Views - Unordered Access Views) permettant d'acc�der � des ressources du shader
	D3D12_DESCRIPTOR_HEAP_DESC cbvSrvUavHeapDesc = {};
	cbvSrvUavHeapDesc.NumDescriptors = 1;
	cbvSrvUavHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	cbvSrvUavHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

	// Cr�ation du tas de descripteurs CBV_SRV_UAV dont le shader a besoin pour acc�der aux diff�rentes ressources
	Engine::Device->CreateDescriptorHeap(&cbvSrvUavHeapDesc, IID_PPV_ARGS(&m_descriptorHeaps));

	// Stockage du constant buffer view dans le tas
	Engine::Device->CreateConstantBufferView(&constBufferView, m_descriptorHeaps->GetCPUDescriptorHandleForHeapStart());
}

ConstantBuffer::~ConstantBuffer()
{
	delete m_descriptorHeaps;
}

void ConstantBuffer::Update(ConstantBufferData* constBufferData)
{
	UpdateBuffer(constBufferData);
}
