#include "Transform.h"
#include "framwork.h"
#include "Camera.h"

Camera::Camera()
{
	m_fov = XM_PIDIV2;
	m_aspectRatio = 16 / 9.0f;
	m_nearPlane = 0.1f;
	m_farPlane = 1000.0f;
	XMStoreFloat4x4(&m_projMatrix, XMMatrixPerspectiveFovLH(m_fov, m_aspectRatio, m_nearPlane, m_farPlane));
	
	this->AddComponent<Transform>();

	Update();
}

Camera::~Camera()
{
}

void Camera::Update()
{
	if (this->GetComponent<Transform>()->IsDirty())
	{
		XMStoreFloat4x4(&m_viewMatrix, this->GetComponent<Transform>()->GetMatrixTranspose());
	}
}

void Camera::CreateFrustum()
{
	m_frustum = new BoundingFrustum;
	m_frustum->CreateFromMatrix(*m_frustum, XMLoadFloat4x4(&m_projMatrix));
}

Camera* Camera::GetInstance()
{
	static Camera instance;
	return &instance;
}
