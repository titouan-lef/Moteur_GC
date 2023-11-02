#pragma once
#include "Buffer.h"

struct ConstantBufferData
{
	DirectX::XMMATRIX World;
	DirectX::XMMATRIX View;
	DirectX::XMMATRIX Projection;
};


class ConstantBuffer : public Buffer
{
public:
	ConstantBuffer(ConstantBufferData* constBufferData);
	virtual ~ConstantBuffer();

	void Update(ConstantBufferData* constBufferData);

public://TO DO : en private ?
	ID3D12DescriptorHeap* m_descriptorHeaps = nullptr;// Tas de descripteurs dont le shader a besoin pour accéder aux différentes ressources
};

