#pragma once

#include "framwork.h"

class Entity;

class Component
{
public:
	Component();
	virtual ~Component();

	virtual void Initialize() {}

	bool inline IsActive() { return m_isActive; }

	void inline Activate() { m_isActive = true; OnActivate(); }
	void inline Deactivate() { m_isActive = false; OnDeactivate(); }

	virtual void Update() {}
	virtual void Render() {}

	void inline SetOwner(Entity* entity) { m_owner = entity; }
	Entity* GetOwner() { return m_owner; }

	std::string GetName() { return m_name; }
	
protected:
	std::string m_name;
	// Sert � savoir si le component a �t� modifi� depuis la derni�re frame
	bool m_isDirty;
	// Sert � savoir si le component est actif ou non
	bool m_isActive;

	Entity* m_owner = nullptr;

	virtual void OnActivate() {}
	virtual void OnDeactivate() {}
};