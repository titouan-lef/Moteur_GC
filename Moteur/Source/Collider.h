#pragma once
#include "Component.h"

class Transform;

class Collider : public Component
{
public:
	Collider();
	virtual ~Collider();

	virtual void Initialize()override;
	virtual void Update();
	Transform* GetCollider();

	bool CheckCollision(Collider& collider1, Collider& collider2);
	bool IsInGameArea() { return m_isInGameArea; }
private:
	bool m_isInGameArea;
	Transform* m_Collider;
};

