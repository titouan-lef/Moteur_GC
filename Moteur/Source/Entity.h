#pragma once
#include "Transform.h"

class Component;

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

	// Prevent copying parent/child or nullptr
	bool CheckAddChild(Entity* child);
	// Prevent copying components
	bool CheckAddComponent(Component* component);
};

template <typename T>
T* Entity::AddComponent()
{
	T* component = new T();
	if (!CheckAddComponent(component)) return nullptr;
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