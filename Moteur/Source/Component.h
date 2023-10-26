#pragma once

#include "framwork.h"
#include "Entity.h"

class Component
{
public:
	Component();
	virtual ~Component();

	virtual void Initialize() = 0;

	bool inline IsActive() { return m_isActive; }
	bool inline IsDirty() { return m_isDirty; }

	void inline Activate() { m_isActive = true; OnActivate(); }
	void inline Deactivate() { m_isActive = false; OnDeactivate(); }

	virtual void Update() = 0;

	void inline SetEntity(Entity* entity) { m_entity = entity; }
protected:
	bool m_isDirty;
	bool m_isActive;
	Entity* m_entity;

	virtual void OnActivate() = 0;
	virtual void OnDeactivate() = 0;
};