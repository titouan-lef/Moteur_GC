#pragma once

#include "framwork.h"
#include "Component.h"

using namespace DirectX;

class Entity
{
public:
	Entity();
	virtual ~Entity();

	TRANSFORM m_Transform;
	std::vector<Component*> m_Components;

	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void PostUpdate() = 0;
	virtual void Render() = 0;
	virtual void OnDestroy() = 0;

	static void AddComponent(Entity* entity, Component* component);

	void RealUpdate();
protected:

private:
};