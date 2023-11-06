#pragma once
#include "ConstantBuffer.h"
struct Light {
    DirectX::XMFLOAT3 position;
    DirectX::XMFLOAT3 intensity;
    DirectX::XMFLOAT3 direction;
};
class ConstantBufferLight : public ConstantBuffer
{
private:
    Light m_light;
public:
	ConstantBufferLight(ConstantBufferData* constBufferData, Light light);
	void Update(const Light& lightData);
};

