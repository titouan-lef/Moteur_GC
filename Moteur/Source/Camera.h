#pragma once
#include "Entity.h"

class Camera : public Entity
{
public:
	Camera();
	virtual ~Camera();

	virtual void Update() override;	

#pragma region Gets/Sets
	static Camera* GetInstance();
	inline XMMATRIX GetViewMatrix() { return XMLoadFloat4x4(&m_viewMatrix); }
	inline XMMATRIX GetTransposedView() { return XMMatrixTranspose(XMLoadFloat4x4(&m_viewMatrix)); }
	inline XMMATRIX GetProjMatrix() { return XMLoadFloat4x4(&m_projMatrix); }
	inline XMMATRIX GetTransposedProj() { return XMMatrixTranspose(XMLoadFloat4x4(&m_projMatrix)); }
	inline XMMATRIX GetViewProjMatrix() { return XMMatrixMultiply(XMLoadFloat4x4(&m_viewMatrix), XMLoadFloat4x4(&m_projMatrix)); }
#pragma endregion

protected:
	float m_fov = -1;
	float m_aspectRatio = -1;
	float m_nearPlane = -1;
	float m_farPlane = -1;

	XMFLOAT4X4 m_viewMatrix;
	XMFLOAT4X4 m_projMatrix;
};