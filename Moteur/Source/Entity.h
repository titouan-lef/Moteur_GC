#pragma once

#include "framwork.h"
#include "Transform.h"

class Component;

struct ConstantBufferData
{
	//DirectX::XMMATRIX View;
	DirectX::XMMATRIX World;
	//DirectX::XMMATRIX Projection;
};

class Entity
{
public:
	Entity();
	virtual ~Entity();

	int m_ID;

	std::vector<Entity*> m_Children;
	std::vector<Component*> m_Components;

	virtual void Init() {}
	virtual void Update() {}
	virtual void PostUpdate() {}
	virtual void Render() {}
	virtual void OnDestroy() {}

	void AddChild(Entity* child);

	template <typename T>// = Component>
	T* AddComponent();

	template <typename T>// = Component>
	T* GetComponent();

	void RealUpdate();
protected:
	Entity* m_Parent;

	void SetParent(Entity* parent) { m_Parent = parent; }
private:
	UINT constBufferSize;
	ConstantBufferData* constBufferData;
	ID3D12Resource* constBuffer;
	std::vector<ID3D12DescriptorHeap*> descriptorHeaps = {}; // Tableau de tas de descripteurs dont le shader a besoin pour accéder aux différentes ressources (1 tas par constant buffer)


	// Create a buffer on the GPU and copy the contents of the CPU buffer into it
	void CreateBuffer();
	// Update the constant buffer
	void UpdateConstBuffer();
	// Create a view of the buffer on the GPU
	D3D12_CONSTANT_BUFFER_VIEW_DESC* CreateConstantBufferView();
	// Create descriptor heap
	void CreateDescriptorHeap(D3D12_CONSTANT_BUFFER_VIEW_DESC* cbvDesc);
	// Prevent copying parent/child or nullptr
	bool CheckAddChild(Entity* child);
	// Prevent copying components
	bool CheckAddComponent(Component* component);
};

template <typename T>
T* Entity::AddComponent()
{
	T* component = new T();
	//if (!CheckAddComponent(component)) return nullptr;
	m_Components.push_back(dynamic_cast<Component*>(component));
	return component;
}

template <typename T>
T* Entity::GetComponent()
{
	for (auto it = m_Components.begin(); it != m_Components.end(); ++it)
	{
		if (dynamic_cast<T*>(*it) != nullptr)
			return dynamic_cast<T*>(*it);
	}
	return nullptr;
}