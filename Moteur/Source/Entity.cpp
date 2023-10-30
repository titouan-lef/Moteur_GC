#include "framwork.h"
#include "Component.h"
#include "Entity.h"

Entity::Entity()
{
	m_ID = rand() % 1000000;
	constBufferData = new ConstantBufferData();
	constBufferData->World = GetComponent<Transform>()->GetMatrixTranspose();
	
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
