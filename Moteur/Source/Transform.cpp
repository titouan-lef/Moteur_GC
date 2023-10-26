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
}

void Transform::Update()
{
	if (IsDirty)
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
}

void Transform::RotateYaw(float angle)
{
	// Effectue une rotation autour de l'axe Z
	XMStoreFloat4(&RotationQuat, XMQuaternionRotationAxis(XMLoadFloat3(&Dir), angle));
}

void Transform::RotatePitch(float angle)
{
	// Effectue une rotation autour de l'axe Y
	XMStoreFloat4(&RotationQuat, XMQuaternionRotationAxis(XMLoadFloat3(&Right), angle));
}

void Transform::RotateRoll(float angle)
{
	// Effectue une rotation autour de l'axe X
	XMStoreFloat4(&RotationQuat, XMQuaternionRotationAxis(XMLoadFloat3(&Up), angle));
}