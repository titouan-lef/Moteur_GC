#include "Entity.h"
#include "Transform.h"
#include "Camera.h"

Camera::Camera()
{
}

void Camera::Init()
{
	m_fov = XM_PIDIV2;
	m_aspectRatio = 16/9.0f;
	m_nearPlane = 0.1f;
	m_farPlane = 1000.0f;		
	XMStoreFloat4x4(&m_projMatrix, 
		XMMatrixPerspectiveFovLH(m_fov, m_aspectRatio, m_nearPlane, m_farPlane));
	m_transform = new Transform;
	AddComponent(m_transform);
}

void Camera::Update()
{
	if (m_transform->IsDirty())
	{
		XMStoreFloat4x4(&m_viewMatrix, m_transform->GetMatrixTranspose());
	}
}
