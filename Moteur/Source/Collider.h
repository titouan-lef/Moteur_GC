#pragma once
#include "Component.h"
#include "Transform.h"
class Collider : public Component
{
public:
	Collider();
	virtual void Update();
	Transform* GetCollider();
	void SetCollider(const Transform& colliderTransform);
	bool CheckCollision(Collider& collider1, Collider& collider2);
private:
	Transform* m_Collider;
};

