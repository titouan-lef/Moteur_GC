#include "Transform.h"
#include "Camera.h"
#include "Engine.h"

Transform::Transform()
{
	Scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
	Dir = XMFLOAT3(0.0f, 0.0f, 1.0f);
	Right = XMFLOAT3(1.0f, 0.0f, 0.0f);
	Up = XMFLOAT3(0.0f, 1.0f, 0.0f);
	RotationQuat = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	Position = XMFLOAT3(0.0f, 0.0f, -1.0f);
	Matrix = XMFLOAT4X4();
	m_isDirty = true;
	m_isActive = true;
	m_name = "Transform";
	m_owner = nullptr;
	this->UpdateMatrix();
}

void Transform::Update()
{
	if (m_isDirty)
	{
		UpdateMatrix();
		UpdateRotMatrix();
		m_isDirty = false;
	}

	CheckIfOnScreen();
}

void Transform::MoveByVector(XMFLOAT3 vec, float elapsed)
{
	Position.x += vec.x * elapsed;
	Position.y += vec.y * elapsed;
	Position.z += vec.z * elapsed;
	m_isDirty = true;
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
	// Effectue une rotation à partir des angles yaw, pitch et roll à partir du quaternion actuelle
	XMMATRIX rotation = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);
	XMStoreFloat4(&RotationQuat, XMQuaternionMultiply(XMLoadFloat4(&RotationQuat), XMQuaternionRotationMatrix(rotation)));
	m_isDirty = true;
}

void Transform::RotateYaw(float angle)
{
	// Effectue une rotation autour de l'axe Z
	XMMATRIX rotation = XMMatrixRotationAxis(XMLoadFloat3(&Up), angle);
	XMStoreFloat4(&RotationQuat, XMQuaternionMultiply(XMLoadFloat4(&RotationQuat), XMQuaternionRotationMatrix(rotation)));
	m_isDirty = true;
}

void Transform::RotatePitch(float angle)
{
	// Effectue une rotation autour de l'axe Y
	XMMATRIX rotation = XMMatrixRotationAxis(XMLoadFloat3(&Right), angle);
	XMStoreFloat4(&RotationQuat, XMQuaternionMultiply(XMLoadFloat4(&RotationQuat), XMQuaternionRotationMatrix(rotation)));
	m_isDirty = true;
}

void Transform::RotateRoll(float angle)
{
	// Effectue une rotation autour de l'axe X
	XMMATRIX rotation = XMMatrixRotationAxis(XMLoadFloat3(&Dir), angle);
	XMStoreFloat4(&RotationQuat, XMQuaternionMultiply(XMLoadFloat4(&RotationQuat), XMQuaternionRotationMatrix(rotation)));
	m_isDirty = true;
}

void Transform::ChangeDirection(Transform* transform) {
	XMFLOAT3 direction = transform->GetDirection();
	direction.x = -direction.x;
	direction.y = -direction.y;
	direction.z = -direction.z;
	transform->SetDirection(direction.x, direction.y, direction.z);
}

void Transform::CheckIfOnScreen()
{
	Camera::GetInstance()->CreateFrustum();

	const XMMATRIX view = Camera::GetInstance()->GetViewMatrix();
	const XMMATRIX invView = XMMatrixInverse(nullptr, view);

	BoundingSphere sphere;
	sphere.Center = Position;
	float scale = 0.0f;

	if (Scale.x > Scale.y && Scale.x > Scale.z)
		scale = Scale.x;
	else if (Scale.y > Scale.x && Scale.y > Scale.z)
		scale = Scale.y;
	else
		scale = Scale.z;
	sphere.Radius = scale;
	
	BoundingFrustum frustum;
	Camera::GetInstance()->GetFrustum()->Transform(frustum, invView);

	if (frustum.Contains(sphere) == DirectX::DISJOINT)
		m_isOnScreen = false;
	else
		m_isOnScreen = true;
}

void Transform::UpdateRotMatrix()
{
	XMStoreFloat4x4(&RotationMatrix, XMMatrixRotationQuaternion(XMLoadFloat4(&RotationQuat)));
}