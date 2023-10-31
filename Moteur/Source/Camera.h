#pragma once

class Entity;

class Camera : public Entity
{
public:
	Camera();

	virtual void Init() override;
	virtual void Update() override;	

#pragma region Gets/Sets
	inline XMMATRIX GetViewMatrix() { return XMLoadFloat4x4(&m_viewMatrix); }
	inline XMMATRIX GetProjMatrix() { return XMLoadFloat4x4(&m_projMatrix); }
	inline XMMATRIX GetViewProjMatrix() { return XMMatrixMultiply(XMLoadFloat4x4(&m_viewMatrix), XMLoadFloat4x4(&m_projMatrix)); }
#pragma endregion

protected:
	float m_fov;
	float m_aspectRatio;
	float m_nearPlane;
	float m_farPlane;

	Transform* m_transform;

	XMFLOAT4X4 m_viewMatrix;
	XMFLOAT4X4 m_projMatrix;

private:

};