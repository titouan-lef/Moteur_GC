#include "Transform.h"

Transform::Transform()
{
	Scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
	Dir = XMFLOAT3(0.0f, 0.0f, 1.0f);
	Right = XMFLOAT3(1.0f, 0.0f, 0.0f);
	Up = XMFLOAT3(0.0f, 1.0f, 0.0f);
	RotationQuat = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	Matrix = XMFLOAT4X4();
	m_isDirty = true;
	m_isActive = true;
	m_name = "Transform";
	m_owner = nullptr;
	this->UpdateMatrix();
}

Transform::~Transform()
{
	delete m_owner;
}

void Transform::Update()
{
	if (m_isDirty)
	{
		UpdateMatrix();
		m_isDirty = false;
	}
}

void Transform::MoveByVector(XMFLOAT3 vec)
{
	Position.x += vec.x;
	Position.y += vec.y;
	Position.z += vec.z;
	m_isDirty = true;
}

void Transform::Identity()
{
	XMStoreFloat4x4(&Matrix, XMMatrixIdentity());
}

void Transform::FromMatrix(XMMATRIX* matrix)
{
	// Met à jour la matrice de Transformation à partir d'une matrice de Transformation
	XMStoreFloat4x4(&Matrix, *matrix);
}

void Transform::UpdateMatrix()
{
	XMStoreFloat4x4(&Matrix, XMMatrixAffineTransformation(XMLoadFloat3(&Scale), XMVectorZero(), XMLoadFloat4(&RotationQuat), XMLoadFloat3(&Position)));
}

void Transform::Rotate(float pitch, float yaw, float roll)
{
	// Effectue une rotation à partir des angles yaw, pitch et roll
	XMStoreFloat4(&RotationQuat, XMQuaternionRotationRollPitchYaw(pitch, yaw, roll));
	m_isDirty = true;
}

void Transform::RotateYaw(float angle)
{
	// Effectue une rotation autour de l'axe Z
	XMStoreFloat4(&RotationQuat, XMQuaternionRotationAxis(XMLoadFloat3(&Dir), angle));
	m_isDirty = true;
}

void Transform::RotatePitch(float angle)
{
	// Effectue une rotation autour de l'axe Y
	XMStoreFloat4(&RotationQuat, XMQuaternionRotationAxis(XMLoadFloat3(&Right), angle));
	m_isDirty = true;
}

void Transform::RotateRoll(float angle)
{
	// Effectue une rotation autour de l'axe X
	XMStoreFloat4(&RotationQuat, XMQuaternionRotationAxis(XMLoadFloat3(&Up), angle));
	m_isDirty = true;
}

bool Transform::CheckCollision(Transform& cube1, Transform& cube2) {
	XMFLOAT3 cube1Scale = cube1.GetScale();
	XMFLOAT3 cube1Min = {
		cube1.GetPosition().x - cube1Scale.x / 2.0f,
		cube1.GetPosition().y - cube1Scale.y / 2.0f,
		cube1.GetPosition().z - cube1Scale.z / 2.0f
	};
	XMFLOAT3 cube1Max = {
		cube1.GetPosition().x + cube1Scale.x / 2.0f,
		cube1.GetPosition().y + cube1Scale.y / 2.0f,
		cube1.GetPosition().z + cube1Scale.z / 2.0f
	};

	XMFLOAT3 cube2Scale = cube2.GetScale();
	XMFLOAT3 cube2Min = {
		cube2.GetPosition().x - cube2Scale.x / 2.0f,
		cube2.GetPosition().y - cube2Scale.y / 2.0f,
		cube2.GetPosition().z - cube2Scale.z / 2.0f
	};
	XMFLOAT3 cube2Max = {
		cube2.GetPosition().x + cube2Scale.x / 2.0f,
		cube2.GetPosition().y + cube2Scale.y / 2.0f,
		cube2.GetPosition().z + cube2Scale.z / 2.0f
	};

	// Check sur l'axe X
	bool overlapX = (cube1Min.x <= cube2Max.x && cube1Max.x >= cube2Min.x);

	// Check sur l'axe Y
	bool overlapY = (cube1Min.y <= cube2Max.y && cube1Max.y >= cube2Min.y);

	// Check sur l'axe Z
	bool overlapZ = (cube1Min.z <= cube2Max.z && cube1Max.z >= cube2Min.z);

	return overlapX && overlapY && overlapZ;
}

void Transform::ChangeDirection(Transform* transform) {
	XMFLOAT3 direction = transform->GetDirection();
	direction.x = -direction.x;
	direction.y = -direction.y;
	direction.z = -direction.z;
	transform->SetDirection(direction.x, direction.y, direction.z);
}
