#include "Collider.h"

Collider::Collider()
{
	m_isDirty = true;
	m_isActive = true;
	m_name = "Collider";
	m_owner = nullptr;
	m_Collider = new Transform();
}


void Collider::Update()
{
	m_Collider->Update();
}

Transform* Collider::GetCollider()
{
	return m_Collider;
}

void Collider::SetCollider(const Transform& colliderTransform) {
	if (m_Collider == nullptr) {
		m_Collider = new Transform();
	}
	*m_Collider = colliderTransform;  // Copie les données de colliderTransform dans m_Collider
}


bool Collider::CheckCollision(Collider& collider1, Collider& collider2) {
	XMFLOAT3 cube1Scale = collider1.GetCollider()->GetScale();
	XMFLOAT3 cube1Min = {
		collider1.GetCollider()->GetPosition().x - cube1Scale.x / 2.0f,
		collider1.GetCollider()->GetPosition().y - cube1Scale.y / 2.0f,
		collider1.GetCollider()->GetPosition().z - cube1Scale.z / 2.0f
	};
	XMFLOAT3 cube1Max = {
		collider1.GetCollider()->GetPosition().x + cube1Scale.x / 2.0f,
		collider1.GetCollider()->GetPosition().y + cube1Scale.y / 2.0f,
		collider1.GetCollider()->GetPosition().z + cube1Scale.z / 2.0f
	};

	XMFLOAT3 cube2Scale = collider2.GetCollider()->GetScale();
	XMFLOAT3 cube2Min = {
		collider2.GetCollider()->GetPosition().x - cube2Scale.x / 2.0f,
		collider2.GetCollider()->GetPosition().y - cube2Scale.y / 2.0f,
		collider2.GetCollider()->GetPosition().z - cube2Scale.z / 2.0f
	};
	XMFLOAT3 cube2Max = {
		collider2.GetCollider()->GetPosition().x + cube2Scale.x / 2.0f,
		collider2.GetCollider()->GetPosition().y + cube2Scale.y / 2.0f,
		collider2.GetCollider()->GetPosition().z + cube2Scale.z / 2.0f
	};

	// Check sur l'axe X
	bool overlapX = (cube1Min.x <= cube2Max.x && cube1Max.x >= cube2Min.x);

	// Check sur l'axe Y
	bool overlapY = (cube1Min.y <= cube2Max.y && cube1Max.y >= cube2Min.y);

	// Check sur l'axe Z
	bool overlapZ = (cube1Min.z <= cube2Max.z && cube1Max.z >= cube2Min.z);

	return overlapX && overlapY && overlapZ;
}