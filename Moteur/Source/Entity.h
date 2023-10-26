#pragma once

#include "framwork.h"
#include "Component.h"

using namespace DirectX;

class Entity
{
public:
	Entity();
	virtual ~Entity();

	int m_ID;

	std::vector<Entity*> m_Children;
	std::vector<Component*> m_Components;

	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void PostUpdate() = 0;
	virtual void Render() = 0;
	virtual void OnDestroy() = 0;

	void AddChild(Entity* child);
	void AddComponent(Component* component);

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