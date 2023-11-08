#pragma once
#include "Component.h"

class Transform;

class Collider : public Component
{
public:
	Collider();
	virtual ~Collider();

	virtual void Initialize()override;
	virtual void Update() override;
	Transform* GetCollider();

	bool CheckCollision(Collider* collider);
	virtual void OnCollision(Entity* e);
private:
	Transform* m_Collider;
};