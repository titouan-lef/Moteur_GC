#include "framwork.h"
#include "Component.h"
#include "Entity.h"

Entity::Entity()
{
	m_ID = rand() % 1000000;
	constBufferData = new ConstantBufferData();
	constBufferData->World = GetComponent<Transform>()->GetMatrixTranspose();
	constBufferSize = (sizeof(ConstantBufferData) + 255) & ~255;
	constBuffer = nullptr;
	CreateBuffer();
	CreateDescriptorHeap(cbvDesc);
}

Entity::~Entity()
{
	for (auto it = m_Components.begin(); it != m_Components.end(); ++it)
	{
		delete *it;
	}
	m_Components.clear();
}

void Entity::AddChild(Entity* child)
{
	if (!CheckAddChild(child)) return;
	this->m_Children.push_back(child);
	child->SetParent(this);
}

void Entity::RealUpdate()
{
	Update();
	PostUpdate();
	for (auto it = m_Components.begin(); it != m_Components.end(); ++it)
	{
		if ((*it)->IsActive())
			(*it)->Update();
	}
}

bool Entity::CheckAddChild(Entity* child)
{
	if (child == nullptr)
	{
		MessageBox(NULL, L"Child is nullptr", L"Error", MB_OK);
		return false;
	}
	if (child->m_Parent != nullptr)
	{
		MessageBox(NULL, L"Child already has a parent", L"Error", MB_OK);
		return false;
	}
	return true;
}

bool Entity::CheckAddComponent(Component* component)
{
	// Check if component is nullptr
	if (component == nullptr)
	{
		MessageBox(NULL, L"Component is nullptr", L"Error", MB_OK);
		return false;
	}

	//Check if there is already a component of the same type
	for (Component* comp : m_Components)
	{
		if (typeid(*comp) == typeid(*component))
		{
			MessageBox(NULL, L"Component already exists", L"Error", MB_OK);
			return false;
		}
	}
	return true;
}

void Entity::CreateBuffer()
{
	constBuffer = nullptr;
	auto tmp1 = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	auto tmp2 = CD3DX12_RESOURCE_DESC::Buffer(constBufferSize);
	GFX_THROW_INFO_ONLY(m_device->CreateCommittedResource(
		&tmp1,
		D3D12_HEAP_FLAG_NONE,
		&tmp2,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffer)));

	UpdateConstBuffer();
}

void Entity::UpdateConstBuffer()
{
	// Copie des données dans le buffer
	BYTE* mappedData = nullptr;
	GFX_THROW_INFO_ONLY(constBuffer->Map(0, nullptr, reinterpret_cast<void**>(&mappedData)));
	memcpy(mappedData, constBufferData, constBufferSize);
	constBuffer->Unmap(0, nullptr);
}

void Entity::CreateDescriptorHeap(D3D12_CONSTANT_BUFFER_VIEW_DESC* cbvDesc)
{
	D3D12_CONSTANT_BUFFER_VIEW_DESC* cbvDesc = CreateConstantBufferView();

	// Propriétés du tas de descripteurs CBV_SRV_UAV (Constant Buffer View - Shader Resource Views - Unordered Access Views) permettant d'accéder à des ressources du shader
	D3D12_DESCRIPTOR_HEAP_DESC cbvSrvUavHeapDesc = {};
	cbvSrvUavHeapDesc.NumDescriptors = 1;
	cbvSrvUavHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	cbvSrvUavHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

	// Création du tas de descripteurs CBV_SRV_UAV dont le shader a besoin pour accéder aux différentes ressources
	ID3D12DescriptorHeap* cbvSrvUavHeap = nullptr;
	m_device->CreateDescriptorHeap(&cbvSrvUavHeapDesc, IID_PPV_ARGS(&cbvSrvUavHeap));

	// Stockage du constant buffer view dans le tas
	m_device->CreateConstantBufferView(&cbvDesc, cbvSrvUavHeap->GetCPUDescriptorHandleForHeapStart());

	descriptorHeaps.push_back(cbvSrvUavHeap);
}

D3D12_CONSTANT_BUFFER_VIEW_DESC* Entity::CreateConstantBufferView()
{
	// Défini l'emplacement et la taille des données du constant buffer
	D3D12_CONSTANT_BUFFER_VIEW_DESC constBufferView = {};
	constBufferView.BufferLocation = constBuffer->GetGPUVirtualAddress();		// Localisation du constant buffer
	constBufferView.SizeInBytes = constBufferSize;											// Taille du constant buffer

	return &constBufferView;
}
