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
	ConstantBuffer(ConstantBufferData* constBufferData, UINT nbDescriptor = 1);
	virtual ~ConstantBuffer();

	void UpdateBuffer(XMMATRIX world);

	virtual void SetGraphicsRoot();

public://TO DO : en private ?
	ID3D12DescriptorHeap* m_cbvHeapDesc = nullptr;// Tas de descripteurs dont le shader a besoin pour acc�der aux diff�rentes ressources (il est de type cbv ou cbv-srv
};

