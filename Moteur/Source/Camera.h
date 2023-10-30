#pragma once

class Entity;

class Camera : public Entity
{
public:
	Camera();

	virtual void Init() override;
	virtual void Update() override;	

#pragma region Gets/Sets
	inline XMFLOAT4X4 GetViewMatrix() { return m_viewMatrix; }
	inline XMFLOAT4X4 GetProjMatrix() { return m_projMatrix; }
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